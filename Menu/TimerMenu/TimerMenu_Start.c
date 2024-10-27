#include "TimerMenu_Start.h"
#include "Menu.h"
#include "TimerMenu.h"

#include "Message.h"
#include "oled.h"
#include "MyTime.h"


#include <stdio.h>
#include <stdlib.h>

Menu timerMenuStart;

static const char *timerMenuStartName = "开始计时";
// 图标 播放
static const uint8_t timerMenuStartIcon[] = {
    0x00, 0xf0, 0xf8, 0xfc, 0x7e, 0x3e, 0x1f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0f, 0x0f, 0x0f, 0x1f, 0x3e, 0x7e, 0xfc, 0xf8, 0xf0, 
    0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
    0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xfc, 
    0xf8, 0xf8, 0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
    0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 
    0x3f, 0x1f, 0x1f, 0x0f, 0x0f, 0x07, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
    0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
    0x00, 0x0f, 0x1f, 0x3f, 0x7e, 0x7c, 0xf8, 0xf0, 0xf0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 
    0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf8, 0x7c, 0x7e, 0x3f, 0x1f, 0x0f, 
};
MyTime myTime;
MyTime myTimeBak;
EventGroupHandle_t updateEvent;
TimerHandle_t myTimerHandle;

static void Timer_Task(TimerHandle_t xTimer)
{
    (void)xTimer;
    MyTimeDec(&myTime);
    xEventGroupSetBits(updateEvent, 0x01);
}

static void CreateViewStatus0()
{
    OLED_NewFrame();
    OLED_DrawLine(0, 16, 128, 16);
    OLED_ShowChinese(0, 0, "状态");
    OLED_ShowChar(32, 0, ':');

    if (timerStatus == TS_STOPPED) {
        OLED_ShowChinese(96, 0, "停止");
    } else if (timerStatus == TS_COUNT_DOWN) {
        OLED_ShowChinese(96, 0, "向下");
    } else if (timerStatus == TS_COUNT_UP) {
        OLED_ShowChinese(96, 0, "向上");
    } else if (timerStatus == TS_PAUSED) {
        OLED_ShowChinese(96, 0, "暂停");
    }

    OLED_ShowFrame();
}

static int CreateViewConfirm0()
{
    OLED_NewFrame();
    OLED_ShowChinese(0, 0, "时间是");
    OLED_Printf(0, 16, "%02d", myTime.minute);
    OLED_ShowChinese(16, 16, "分");
    OLED_Printf(32, 16, "%02d", myTime.second);
    OLED_ShowChinese(48, 16, "秒");

    OLED_ShowChinese(0, 32, "按");
    OLED_ShowChar(16, 32, '2');
    OLED_ShowChinese(24, 32, "开始");
    OLED_ShowChinese(0, 48, "其他结束");

    OLED_ShowFrame();

    MSG msg;
    while (1)
    {
        MQ_GetMessage(&msg);
        if (msg.msgType == CM_KEYUP) {
            if (msg.msgParam == VK_KEY2) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    
    return 0;
}

static int SetupTimerView(void)
{
    OLED_NewFrame();
    OLED_ShowChinese(0, 0, "请输入时间");
    OLED_DrawRectangle(0, 32, 26, 26, 0);
    OLED_DrawRectangle(30, 32, 26, 26, 0);
    // 时间中间的:
    OLED_ShowChar(58, 32, ':');
    OLED_DrawRectangle(70, 32, 26, 26, 0);
    OLED_DrawRectangle(100, 32, 26, 26, 0);
    OLED_ShowFrame();

    char buffer[4] = {0};
    int inputOffset = 0;
    MSG msg;
    while (1)
    {
        // 从队列读取按键
        if (MQ_GetMessage(&msg) > 0) {
            if (msg.msgType == CM_KEYUP) {
                if ((msg.msgParam >= VK_KEY1 && msg.msgParam <= VK_KEY9) || msg.msgParam == VK_KEY0) {
                    if (msg.msgParam == VK_KEY0) {
                        msg.msgParam = 0;
                    }
                    buffer[inputOffset] = '0' + msg.msgParam;
                    int starXPos = 0;
                    if (inputOffset == 0) {
                        starXPos = 10;
                    } else if (inputOffset == 1) {
                        starXPos = 40;
                    } else if (inputOffset == 2) {
                        starXPos = 80;
                    } else if (inputOffset == 3) {
                        starXPos = 110;
                    }
                    OLED_ShowChar(starXPos, 33, buffer[inputOffset]);
                    inputOffset++;
                }
            }
            OLED_ShowFrame();
        }

        if (inputOffset == 4) {
            break;
        }
    }
    // 取分和秒到结构体
    char tmpBuffer[4] = {0};
    tmpBuffer[0] = buffer[0];
    tmpBuffer[1] = buffer[1];
    tmpBuffer[2] = 0;
    myTime.minute = atoi(tmpBuffer);

    tmpBuffer[0] = buffer[2];
    tmpBuffer[1] = buffer[3];
    tmpBuffer[2] = 0;
    myTime.second = atoi(tmpBuffer);

    printf("minute: %d, second: %d\r\n", myTime.minute, myTime.second);

    return 0;
}

static int ShowTimerProgressView()
{
    MSG msg;
    char buf[4] = {0};
    if (timerStatus == TS_COUNT_DOWN) {
        // 画一个进度条
        int origSec = MyTime2Seconds(&myTimeBak);
        int nowSec = MyTime2Seconds(&myTime);
        double val = 128.0 * nowSec / origSec;
        OLED_ClearArea(0, 22, 128, 12);
        OLED_DrawRectangle(0, 24, (int)val, 8, 1);
        OLED_DrawRectangle(0, 22, 128, 12, 0);
    }

    OLED_DrawRectangle(0, 38, 26, 26, 0);
    OLED_DrawRectangle(30, 38, 26, 26, 0);
    // 时间中间的:
    OLED_ShowChar(58, 38, ':');
    OLED_DrawRectangle(70, 38, 26, 26, 0);
    OLED_DrawRectangle(100, 38, 26, 26, 0);
    
    sprintf(buf, "%02d", myTime.minute);
    OLED_ShowChar(10, 39, buf[0]);
    OLED_ShowChar(40, 39, buf[1]);
    sprintf(buf, "%02d", myTime.second);
    OLED_ShowChar(80, 39, buf[0]);
    OLED_ShowChar(110, 39, buf[1]);
    xEventGroupWaitBits(updateEvent, 0x03, pdTRUE, pdFALSE, portMAX_DELAY);
    if (MQ_GetMessage(&msg) > 0) {
        if (msg.msgType == CM_KEYUP) {
            return 1;
        }
    }
    return 0;
}


void TimerMenuStart_Function()
{
    MSG msg;
    MSG_Init(&msg);

    // 显示状态
    CreateViewStatus0();
    while (1)
    {
        if (timerStatus == TS_STOPPED) {
            OLED_ShowChinese(0, 16, "按");
            OLED_ShowChar(16, 16, '8');
            OLED_ShowChinese(24, 16, "退出");
            OLED_ShowChinese(0, 32, "其他按键设置时间");
            if (MQ_GetMessage(&msg)) {
                // 按8返回
                if (msg.msgType == CM_KEYUP && msg.msgParam == VK_KEY8) {
                    return;
                } else if (msg.msgType == CM_KEYUP && msg.msgParam != VK_KEY8) {    // 按其他进行继续操作
                    SetupTimerView();
                    FixMyTime(&myTime);
                    int confirmResult = CreateViewConfirm0();
                    if (confirmResult == 0) {
                        CreateViewStatus0();
                        OLED_ShowChinese(0, 32, "您选择了结束");
                        OLED_ShowFrame();
                        vTaskDelay(1000);
                    } else if (confirmResult == 1) {
                        timerStatus = TS_COUNT_DOWN;
                        myTimeBak = myTime;
                        xTimerStart(myTimerHandle, 0);
                        CreateViewStatus0();
                        OLED_ShowChinese(0, 32, "已经开始");
                        OLED_ShowFrame();
                        vTaskDelay(1000);

                        OLED_NewFrame();
                        CreateViewStatus0();
                        while (1)
                        {
                            if (ShowTimerProgressView() > 0) {
                                break;
                            }
                            OLED_ShowFrame();
                        }
                    }
                    return;
                }
            }
        } else if (timerStatus == TS_COUNT_DOWN || timerStatus == TS_COUNT_UP) {
            if (ShowTimerProgressView() > 0) {
                return;
            }
        }
        OLED_ShowFrame();
        vTaskDelay(50);
    }
}

void TimerMenuStart_Init()
{
    Menu_Init(&timerMenuStart);
    Menu_SetName(&timerMenuStart, timerMenuStartName);
    Menu_SetIcon(&timerMenuStart, timerMenuStartIcon);
    Menu_SetFunction(&timerMenuStart, &TimerMenuStart_Function);

    updateEvent = xEventGroupCreate();
    myTimerHandle = xTimerCreate("TIMER_TASK", 1000, pdTRUE, (void *)1, Timer_Task);
}