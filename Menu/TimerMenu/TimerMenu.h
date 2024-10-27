#ifndef __TIMER_MENU_H__
#define __TIMER_MENU_H__

#include "Menu.h"
#include "MyTime.h"

extern Menu timerMenu;

void TimerMenu_Function();
void TimerMenu_Init();

int TimerMenu_GetTimerStatus();
void TimerMenu_SetTimerStatus(int newStatus);

#endif