#include "stm32f103xe.h"
#include "keyboard.h"
#include "Message.h"

#include <stdint.h>

#include "FreeRTOS.h"
#include "event_groups.h"

extern EventGroupHandle_t updateEvent;

struct KBD_Pin {
    GPIO_TypeDef *gpioPort;
    uint16_t gpioPin;
};
typedef struct KBD_Pin KBD_Pin_t;

static int kbdStatusArray[4][3] = {0};
static KBD_Pin_t KBD_rows[] = {{KBD_R1_GPIO_Port, KBD_R1_Pin}, {KBD_R2_GPIO_Port, KBD_R2_Pin}, {KBD_R3_GPIO_Port, KBD_R3_Pin}, \
    {KBD_R4_GPIO_Port, KBD_R4_Pin}};
static KBD_Pin_t KBD_cols[] = {{KBD_C1_GPIO_Port, KBD_C1_Pin}, {KBD_C2_GPIO_Port, KBD_C2_Pin}, {KBD_C3_GPIO_Port, KBD_C3_Pin}};


void KBD_Init()
{
    // 所有行先初始化为高电平
    for (int i = 0; i < KBD_ROWS; i++) {
        HAL_GPIO_WritePin(KBD_rows[i].gpioPort, KBD_rows[i].gpioPin, GPIO_PIN_SET);
    }
}

int KBD_Scan()
{
    uint8_t keyNum = VK_UNPRESS;

    // 扫描每一行
    for (int i = 0; i < KBD_ROWS; i++) {
        // 拉低
        HAL_GPIO_WritePin(KBD_rows[i].gpioPort, KBD_rows[i].gpioPin, GPIO_PIN_RESET);
        // 扫描每一列
        for (int j = 0; j < KBD_COLS; j++) {
            if (HAL_GPIO_ReadPin(KBD_cols[j].gpioPort, KBD_cols[j].gpioPin) == GPIO_PIN_RESET) {
                kbdStatusArray[i][j] = 1;
            } else {
                kbdStatusArray[i][j] = 0;
            }
        }
        HAL_GPIO_WritePin(KBD_rows[i].gpioPort, KBD_rows[i].gpioPin, GPIO_PIN_SET);
    }

    for (int i = 0; i < KBD_ROWS; i++) {
        for (int j = 0; j < KBD_COLS; j++) {
            if (kbdStatusArray[i][j] == 1) {
                int tmp = i*3+(j+1);
                keyNum = tmp;
            }
        }
    }
    // 没有键按下
    return keyNum;
}

int KBD_Loop()
{
    static uint8_t nowState, lastState;
    MSG msg;

    lastState = nowState;
    nowState = KBD_Scan();

    // 按键按下
    if (lastState == VK_UNPRESS && nowState == VK_KEY1) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY1;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);
    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY2) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY2;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);
    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY3) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY3;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY4) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY4;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY5) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY5;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY6) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY6;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY7) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY7;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY8) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY8;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY9) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY9;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY_STAR) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY_STAR;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY0) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY0;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY_HASH) {
        msg.msgType = CM_KEYDOWN;
        msg.msgParam = VK_KEY_HASH;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }

    // 按键抬起
    if (nowState == VK_KEY1 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY1;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (lastState == VK_UNPRESS && nowState == VK_KEY2 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY2;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY3 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY3;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY4 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY4;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY5 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY5;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY6 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY6;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY7 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY7;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY8 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY8;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY9 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY9;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY_STAR && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY_STAR;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY0 && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY0;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }
    if (nowState == VK_KEY_HASH && lastState == VK_UNPRESS) {
        msg.msgType = CM_KEYUP;
        msg.msgParam = VK_KEY_HASH;
        msg.msgCount = 1;
        MQ_SendMessage(&msg);
        xEventGroupSetBits(updateEvent, 0x02);

    }

    return 0;
}