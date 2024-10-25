#include "Message.h"

#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"

static MQ mq;
static uint8_t mqInitFlag = 0;

static SemaphoreHandle_t binarySemaphore;
#define MQ_LOCK()   xSemaphoreTake(binarySemaphore, portMAX_DELAY)
#define MQ_UNLOCK() xSemaphoreGive(binarySemaphore)

static uint8_t MQ_IsEmpty()
{
    return mq.front == mq.rear;
}

static uint8_t MQ_IsFull()
{
    return (mq.rear + 1) % mq.capacity == mq.front;
}

static void MQ_SendInitMessage(void)
{
    MSG msg = {
        .msgType = CM_Init,
        .msgParam = 0,
        .msgCount = 1
    };

    mq.data[mq.rear] = msg;
    mq.rear = (mq.rear + 1) % mq.capacity;
}

void MQ_Init()
{
    if (mqInitFlag == 1) {
        return;
    }

    mq.capacity = MSG_QUEUE_LEN;
    memset(mq.data, 0x00, sizeof(mq.data));
    mq.front = 0;
    mq.rear = 0;

    MQ_SendInitMessage();
    mqInitFlag = 1;

    // 创建二值信号量
    binarySemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(binarySemaphore);
}

uint8_t MQ_SendMessage(MSG *msg)
{
    MQ_LOCK();

    int i = 0;
    // 从最后一个消息搜到第一个消息为止 
    for (i = (mq.rear+mq.capacity-1)%mq.capacity; i != (mq.front+mq.capacity-1)%mq.capacity; i = (i+mq.capacity-1)%mq.capacity) {
        // 若有相同的类型和参数, 且计数小于255
        if (mq.data[i].msgType == msg->msgType && mq.data[i].msgParam == msg->msgParam) {
            if (mq.data[i].msgCount < 255) {
                // 累加计数
                mq.data[i].msgCount++;
                MQ_UNLOCK();
                return 1;
            } else {
                // 否则添加一个新的
                break;
            }
        }
    }

    // 队列已经满了判断
    if (MQ_IsFull()) {
        MQ_UNLOCK();
        return 0;
    } else {
        // 放入队尾
        mq.data[mq.rear] = *msg;
        mq.data[mq.rear].msgCount = 1;
        mq.rear = (mq.rear+1) % mq.capacity;
        MQ_UNLOCK();
        return 1;
    }
}

uint8_t MQ_GetMessage(MSG *msg)
{
    MQ_LOCK();
    if (MQ_IsEmpty()) {
        MQ_UNLOCK();
        return 0;
    }

    *msg = mq.data[mq.front];
    mq.front = (mq.front+1+mq.capacity)%mq.capacity;
    MQ_UNLOCK();
    return 1;
}