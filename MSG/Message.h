#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "MSG.h"

#include <stdint.h>

#define MSG_QUEUE_LEN 128

// ring-buffer
typedef struct MessageQueue {
    uint8_t capacity;                   // 容量
    uint8_t front;                      // 队头
    uint8_t rear;                       // 队尾
    MSG data[MSG_QUEUE_LEN + 1];        // 数据
} MQ;

void MQ_Init();
uint8_t MQ_SendMessage(MSG *msg);
uint8_t MQ_GetMessage(MSG *msg);

#endif