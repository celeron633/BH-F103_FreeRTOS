#ifndef __TIMER_LOGIC_H__
#define __TIMER_LOGIC_H__

#include "i2c.h"
#include "oled.h"

#include <stdio.h>

void timerLogic(void *arg);

void startTimer(void);
void cancelTimer(void);
void setupTime(void);

#endif