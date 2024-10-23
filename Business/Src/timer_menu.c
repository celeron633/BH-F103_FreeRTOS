#include "timer_menu.h"
#include "oled.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include <stdio.h>

extern QueueHandle_t kbdQueue;
extern EventGroupHandle_t kbdEventGroup;

extern KeyTable_t menuFuncs[];
int lastMenuIndex = 0;
int currMenuIndex = 0;
void (*currMenuFunc)(void) = NULL;

#define SELECT_UP()     currMenuIndex = menuFuncs[currMenuIndex].up
#define SELECT_DOWN()   currMenuIndex = menuFuncs[currMenuIndex].down
#define SELECT_ENTER()  currMenuIndex = menuFuncs[currMenuIndex].enter

// 外部函数
void setupTime(void);
void cancelTimer(void);
void startTimer(void);

// static int GetKey()
// {
//     xEventGroupWaitBits(kbdEventGroup, 0x1, pdTRUE, pdTRUE, portMAX_DELAY);
//     int k = 0;
//     xQueueReceive(kbdQueue, (void *)&k, portMAX_DELAY);
//     return k;
// }

static int GetKeyNonBlock()
{
    int k = 0;
    if (xQueueReceive(kbdQueue, (void *)&k, 10) == errQUEUE_EMPTY) {
        return -1;
    }
    return k;
}

void menuLogo()
{
    OLED_NewFrame();
    OLED_ShowString(0, 0, "WELCOME");
    OLED_ShowFrame();
    vTaskDelay(500);
}

void menuMain(void)
{
    OLED_NewFrame();
    OLED_ShowString(16, 16, "START");
    OLED_ShowString(16, 32, "STOP");
    OLED_ShowString(16, 48, "CONFIG");

    switch (lastMenuIndex)
    {
        case 1:
            OLED_ShowString(0, 16, ">");
            break;
        case 2:
            OLED_ShowString(0, 32, ">");
            break;
        case 3:
            OLED_ShowString(0, 48, ">");
            break;
        default:
            break;
    }

    OLED_ShowFrame();
}

void menuEntry()
{
    while (1)
    {
        int k = GetKeyNonBlock();
        printf("k=%d\r\n", k);

        if (k != -1) {
            if (k == 10) {
                SELECT_DOWN();
            } else if (k == 12) {
                SELECT_UP();
            } else if (k == 0) {
                SELECT_ENTER();
            }
        }
        // 欢迎界面
        if (lastMenuIndex == 0) {
            menuFuncs[lastMenuIndex].menuFunc();
            // 自动跳转到主菜单
            SELECT_ENTER();
        }

        // 处理菜单逻辑 选择上, 下, 确认 调用不同的函数
        if (currMenuIndex != lastMenuIndex) {
            lastMenuIndex = currMenuIndex;
            currMenuFunc = menuFuncs[currMenuIndex].menuFunc;
            if (currMenuFunc != NULL) {
                currMenuFunc();
            }
        }
        vTaskDelay(100);
    }
}

KeyTable_t menuFuncs[] = {
    // 启动LOGO
    {0, 0, 0, 1, &menuLogo},

    // 主界面
    {1, 3, 2, 4, &menuMain},
    {2, 1, 3, 5, &menuMain},
    {3, 2, 1, 6, &menuMain},

    // START界面
    {4, 3, 3, 1, &startTimer},

    // CANCEL界面
    {5, 3, 3, 2, &cancelTimer},

    // CONFIG界面
    {6, 3, 3, 3, &setupTime}
};