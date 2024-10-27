#ifndef __TIMER_MENU_H__
#define __TIMER_MENU_H__

#include "Menu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "MyTime.h"

enum TIMER_STATUS {
    TS_STOPPED     = 0,
    TS_COUNT_DOWN  = 1,
    TS_COUNT_UP    = 2,
    TS_PAUSED      = 3,
};

extern Menu timerMenu;
extern int timerStatus;
extern TimerHandle_t myTimerHandle;

void TimerMenu_Function();
void TimerMenu_Init();

int TimerMenu_GetTimerStatus();
void TimerMenu_SetTimerStatus(int newStatus);

#endif