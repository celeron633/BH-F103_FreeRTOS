#ifndef __TIMER_MENU_START_H__
#define __TIMER_MENU_START_H__

#include "Menu.h"
#include "MyTime.h"

#include "FreeRTOS.h"
#include "event_groups.h"

extern MyTime myTime;
extern Menu timerMenuStart; 
extern  EventGroupHandle_t updateEvent;

void TimerMenuStart_Init();

#endif