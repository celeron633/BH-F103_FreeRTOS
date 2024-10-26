#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>

enum{
    GUI_COMMAND_NONE,   // 没指令
    GUI_COMMAND_UP,     // 向上指令
    GUI_COMMAND_DOWN,   // 向下指令
    GUI_COMMAND_LEFT,   // 向左指令
    GUI_COMMAND_RIGHT   // 向右指令
};

int GUI_Init();
void GUI_ReceiveCommand(uint8_t command);
void GUI_Display();

#endif