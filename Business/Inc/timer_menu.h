#ifndef __TIMER_MENU_H__
#define __TIMER_MENU_H__

#include <stdint.h>

typedef struct {
    uint8_t current;
    uint8_t up;
    uint8_t down;
    uint8_t enter;
    void (*menuFunc)(void);
} KeyTable_t;

void menuEntry();

#endif