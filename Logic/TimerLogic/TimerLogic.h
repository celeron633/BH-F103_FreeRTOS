#ifndef __TIMER_LOGIC_H__
#define __TIMER_LOGIC_H__

#include "MyTime.h"

extern int timerStatus;

typedef enum TIMER_STATUS {
    TS_STOPPED     = 0,
    TS_COUNT_DOWN  = 1,
    TS_COUNT_UP    = 2,
    TS_PAUSED      = 3,
} TIMER_STATUS;

void TimerLogic_Init();
void TimerLogic_WaitTimerEvent(void);

MyTime * TimerLogic_GetCurrTime(void);
MyTime * TimerLogic_GetOrigTime(void);
int TimerLogic_GetCurrSec(void);
int TimerLogic_GetOrigSec(void);

void TimerLogic_StartCountDown(void);

TIMER_STATUS TimerLogic_GetTimerStatus(void);
void TimerLogic_SetTimerStatus(TIMER_STATUS status);

#endif