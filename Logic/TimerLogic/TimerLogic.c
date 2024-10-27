#include "TimerLogic.h"

// freeRTOS
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "event_groups.h"

int timerStatus = 0;

static MyTime myTime;
static MyTime myTimeBak;

EventGroupHandle_t updateEvent;
static TimerHandle_t myTimerHandle;

static void TimerCountTask(TimerHandle_t xTimer)
{
    (void)xTimer;
    MyTimeDec(&myTime);
    xEventGroupSetBits(updateEvent, 0x01);
}

void TimerLogic_Init()
{
    updateEvent = xEventGroupCreate();
    myTimerHandle = xTimerCreate("TIMER_TASK", 1000, pdTRUE, (void *)1, TimerCountTask);

    timerStatus = TS_STOPPED;
}

MyTime * TimerLogic_GetCurrTime(void)
{
    return &myTime;
}

MyTime * TimerLogic_GetOrigTime(void)
{
    return &myTimeBak;
}

int TimerLogic_GetCurrSec(void)
{
    return MyTime2Seconds(TimerLogic_GetCurrTime());
}

int TimerLogic_GetOrigSec(void)
{
    return MyTime2Seconds(TimerLogic_GetOrigTime());
}

void TimerLogic_StartCountDown(void)
{
    myTimeBak = myTime;
    xTimerStart(myTimerHandle, 0);
}

void TimerLogic_WaitTimerEvent(void)
{
    xEventGroupWaitBits(updateEvent, 0x03, pdTRUE, pdFALSE, portMAX_DELAY);
}

TIMER_STATUS TimerLogic_GetTimerStatus(void)
{
    return timerStatus;
}

void TimerLogic_SetTimerStatus(TIMER_STATUS status)
{
    timerStatus = status;
}