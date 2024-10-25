#ifndef __MSG_H__
#define __MSG_H__

#include <stdint.h>
#include <stdio.h>

// 类型枚举
enum {
    CM_Init=0,              // 消息队列初始化
    CM_KEYDOWN,             // 按键按下
    CM_KEYUP,               // 按键松开
    CM_TIMEUP,              // 定时器到时了
};

// 参数枚举
enum PARAM
{
    VK_UNPRESS=0,           // 没有按键按下
    VK_KEY1,                // 按键1
    VK_KEY2,                // 按键2
    VK_KEY3,                // 按键3
    VK_KEY4,                // 按键4
    VK_KEY5,                // 按键5
    VK_KEY6,                // 按键6
    VK_KEY7,                // 按键7
    VK_KEY8,                // 按键8
    VK_KEY9,                // 按键9
    VK_KEY_STAR,            // 按键*
    VK_KEY0,                // 按键0
    VK_KEY_HASH             // 按键#
};


typedef struct Message {
    uint8_t msgType;                    // 类型
    uint8_t msgParam;                   // 参数
    uint8_t msgCount;                   // 数量
} MSG;

void MSG_init(MSG *msg);

#endif