#include "timer_logic.h"
#include "MyTime.h"
#include "timer_menu.h"
#include "font.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include <string.h>

// RTOS
extern QueueHandle_t kbdQueue;
extern EventGroupHandle_t kbdEventGroup;
extern EventGroupHandle_t countEventGroup;

// 菜单
extern KeyTable_t menuFuncs[];
extern int lastMenuIndex;
extern int currMenuIndex;

// 计时状态
static char charBuf[8] = {0};
MyTime m = {0, 0};
// 0: stopped, 1: count down, 2: count up
int countStatus = 0;

// 字体
extern const ASCIIFont *currFont;

// 返回上级菜单
#define WILL_BACK_TO_MENU() currMenuIndex = menuFuncs[currMenuIndex].enter

static int GetKey()
{
    xEventGroupWaitBits(kbdEventGroup, 0x1, pdTRUE, pdTRUE, portMAX_DELAY);
    int k = 0;
    xQueueReceive(kbdQueue, (void *)&k, portMAX_DELAY);
    return k;
}

static int GetKeyNonBlock()
{
    int k = 0;
    if (xQueueReceive(kbdQueue, (void *)&k, 10) == errQUEUE_EMPTY) {
        return -1;
    }
    return k;
}

void setupTime(void)
{
setupTime_begin:
    strncpy(charBuf, "__:__", sizeof(charBuf));
    OLED_NewFrame();
    OLED_ShowString(0, 0, "PLEASE INPUT:");
    OLED_ShowString(0, 16, charBuf);
    OLED_ShowFrame();

    memset(&m, 0, sizeof(MyTime));
    while (1)
    {
        charBuf[2]=':';
        for (int i = 0; i < 4; i++) {
            int k = GetKey();
            if (i <= 1)
                charBuf[i] = '0' + k;
            else
                charBuf[i+1] = '0' + k;
            OLED_ShowString(0, 0, "PLEASE INPUT:");
            OLED_ShowString(0, 16, charBuf);
            OLED_ShowFrame();
        }
        Str2MyTime(charBuf, &m);
        FixMyTime(&m);
        printf("minute: %d, second: %d\r\n", m.minute, m.second);
        break;
    }
    
    char buffer[64] = {0};
    sprintf(buffer, "M:%d,S:%d", m.minute, m.second);
    OLED_NewFrame();
    OLED_ShowString(0, 0, "BTN # TO START");
    OLED_ShowString(0, 16, "OTHER TO CANCEL");
    OLED_ShowString(0, 32, buffer);
    OLED_ShowFrame();

    int k = GetKey();
    if (k == 12) {
        WILL_BACK_TO_MENU();
        return;
    } else {
        goto setupTime_begin;
    }
}

void cancelTimer()
{
    if (countStatus == 0) {
        OLED_NewFrame();
        OLED_ShowString(0, 0, "NOT STARTED!");
        OLED_ShowString(0, 16, "PRESS ANY KEY");
        OLED_ShowFrame();
        GetKey();
        WILL_BACK_TO_MENU();
        return;
    }

    strncpy(charBuf, "____", sizeof(charBuf));
    OLED_NewFrame();
    OLED_ShowString(0, 0, "ENTER 4 PASS:");
    OLED_ShowString(0, 16, charBuf);
    OLED_ShowFrame();

    for (int i = 0; i < 4; i++) {
        int k = GetKey();
        charBuf[i] = '0' + k;
        OLED_ShowString(0, 0, "ENTER 4 PASS:");
        OLED_ShowString(0, 16, charBuf);
        OLED_ShowFrame();
    }

    if (charBuf[0] == '1' && charBuf[1] == '2' && charBuf[2] == '3' && charBuf[3] == '4') {
        OLED_NewFrame();
        OLED_ShowString(0, 0, "CORRECT");
        OLED_ShowString(0, 16, "CANCELED");
        OLED_ShowFrame();
        countStatus = 0;
    } else {
        OLED_NewFrame();
        OLED_ShowString(0, 0, "NOT CORRECT");
        OLED_ShowFrame();
    }
    vTaskDelay(500);

    WILL_BACK_TO_MENU();
}

void startTimer(void)
{
    if (countStatus == 0 && m.minute == 0 && m.second == 0) {
        OLED_NewFrame();
        OLED_ShowString(0, 0, "NOT CONFIGED!");
        OLED_ShowString(0, 16, "PRESS ANY KEY");
        OLED_ShowFrame();
        GetKey();
        WILL_BACK_TO_MENU();
        return;
    }

    countStatus = 1;
    
    currFont = &afont24x12;
    while (1)
    {
        int k = GetKeyNonBlock();
        if (k != -1) {
            currFont = &afont16x8;
            WILL_BACK_TO_MENU();
            return;
        }

        // 等待时间刷新
        xEventGroupWaitBits(countEventGroup, 0x01, pdTRUE, pdTRUE, portMAX_DELAY);
        char buffer[24] = {0};
        MyTime2Str(&m, buffer, sizeof(buffer));
        // OLED_ClearArea(0, 16, 128, 48);
        OLED_NewFrame();
        OLED_ShowString(0, 0, "COUNTING...");
        OLED_ShowString(0, 24, buffer);
        OLED_ShowFrame();
    }
    
}

void timerLogic(void *arg)
{
    (void)arg;
    menuEntry();
}