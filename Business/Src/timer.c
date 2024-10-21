#include "timer.h"
#include "my_time.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include <string.h>

extern QueueHandle_t kbdQueue;
extern EventGroupHandle_t kbdEventGroup;
extern I2C_HandleTypeDef hi2c2;

static char charBuf[8] = {0};
MyTime m;
// 0: stopped, 1: count down, 2: count up
int countStatus = 0;

static int GetKey()
{
    xEventGroupWaitBits(kbdEventGroup, 0x1, pdTRUE, pdTRUE, portMAX_DELAY);
    int k = 0;
    xQueueReceive(kbdQueue, (void *)&k, portMAX_DELAY);
    return k;
}

static void DisplayInit()
{
    OLED_ConfigDisplay(&hi2c2, 0x78);
    (void)OLED_InitDisplay();
    OLED_NewFrame();
    OLED_ShowString(0, 0, "TIMER INIT....");
    OLED_ShowFrame();
}

static void SetupTime(void)
{
SetupTime_begin:
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
        return;
    } else {
        goto SetupTime_begin;
    }
}

void TimerLogic(void *arg)
{
    (void)arg;

    // 初始化显示屏
    DisplayInit();
    vTaskDelay(50);

    // 设置时间
    SetupTime();
    // seconds2Count = MyTime2Seconds(&m);
    countStatus = 1;

    char buffer[64] = {0};
    while (1)
    {
        MyTime2Str(&m, buffer, sizeof(buffer));
        OLED_NewFrame();
        OLED_ShowString(0, 0, "ARMED");
        OLED_ShowString(0, 16, "COUNTING...");
        OLED_ShowString(0, 32, buffer);
        OLED_ShowFrame();
        vTaskDelay(100);
    }
}