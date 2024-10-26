#include "GUI.h"

#include "TimerMenu.h"
#include "SettingsMenu.h"

#include "TimerMenu_Start.h"
#include "TimerMenu_Stop.h"

#include <string.h>
#include "oled.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c2;

int GUI_Init()
{
    TimerMenu_Init();
    TimerMenuStart_Init();
    TimerMenuStop_Init();
    SettingsMenu_Init();

    Menu_SetNext(&timerMenu, &settingsMenu);
    Menu_SetCurrent(&timerMenu);

    return 0;
}

void GUI_Display()
{
    OLED_NewFrame();

    // 菜单名称
    if (Menu_GetFather() == NULL) {
        OLED_ShowChinese(0, 0, "菜单");
        OLED_ShowChinese(48, 0, Menu_GetName());
    } else {
        OLED_ShowChinese(  0, 0, Menu_GetFather()->name);
		OLED_ShowChinese( 8*(strlen(Menu_GetFather()->name)/3*2), 0, "：");
		OLED_ShowChinese( 8*(strlen(Menu_GetFather()->name)/3*2)+16, 0, Menu_GetName());
    }

    // 前一个菜单
    if (Menu_GetPrev() != NULL) {
        OLED_ShowImage(-24, 16, 48, 48, Menu_GetPrev()->icon);
    }

    // 本菜单
    OLED_ShowImage(40, 16, 48, 48, Menu_GetCurrent()->icon);

    // 下一个菜单
    if (Menu_GetNext() != NULL) {
        OLED_ShowImage(104, 16, 48, 48, Menu_GetNext()->icon);
    }

    OLED_ShowFrame();
}

// 调用当前菜单的函数(如果有)
static void GUI_ToFunction()
{
    Menu_ToFunction();
}

// 父级菜单
static void GUI_ToFather()
{
    Menu_ToFather();
}

// 子级菜单
static void GUI_ToChild()
{
    Menu_ToChild();
}

// 同级上一个菜单
static void GUI_ToPrev()
{
    Menu_ToPrev();
    /*
    if (Menu_ToPrev() > 0) {
        // 暂时不实现动画效果
        GUI_Display();
    }
    */
}

// 同一级下一个菜单
static void GUI_ToNext()
{
    Menu_ToNext();
    /*
    if (Menu_ToNext() > 0) {
        // 暂时不实现动画效果
        GUI_Display();
    }
    */
}

void GUI_ReceiveCommand(uint8_t command)
{
    switch (command)
    {
    case GUI_COMMAND_NONE:
        /* code */
        break;
    case GUI_COMMAND_UP:
        // 调用当前Menu的函数
        GUI_ToFunction();
        // 跳转到子菜单
        GUI_ToChild();
        break;
    case GUI_COMMAND_DOWN:
        GUI_ToFather();
        break;
    case GUI_COMMAND_LEFT:
        GUI_ToPrev();
        break;
    case GUI_COMMAND_RIGHT:
        GUI_ToNext();
        break;
    default:
        break;
    }
}