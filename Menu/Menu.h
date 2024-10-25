#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>
#include <stdio.h>

typedef void(*MenuFunction)(void);

#define MENU_ICON_WIDTH 48
#define MENU_ICON_HEIGHT    48

typedef struct Menu {
    const char      *name;      // 名字
    const uint8_t   *icon;      // 图标
    MenuFunction    function;   // 功能函数
    struct Menu     *father;    // 父菜单
    struct Menu     *child;     // 子菜单
    struct Menu     *prev;      // 前一个菜单
    struct Menu     *next;      // 后一个菜单
} Menu;

// Init
void Menu_Init(Menu *menu);

// Setter
void Menu_SetName(Menu *menu, const char *name);
void Menu_SetIcon(Menu *menu, const uint8_t *ico);
void Menu_SetFunction(Menu *menu, MenuFunction func);
void Menu_SetFather(Menu *menu, Menu *father);
void Menu_SetChild(Menu *menu, Menu *child);
void Menu_SetNext(Menu *menu, Menu *nextMenu);
void Menu_SetPrev(Menu *menu, Menu *prevMenu);

// for static object (操作Menu.c里面currentMenu指针)
int Menu_ToFather();
int Menu_ToChild();
int Menu_ToNext();
int Menu_ToPrev();

void Menu_SetCurrent(Menu *mu);
Menu * Menu_GetCurrent();
Menu * Menu_GetFather();
Menu * Menu_GetChild();
Menu * Menu_GetPrev();
Menu * Menu_GetNext();

const char * Menu_GetName();
const uint8_t * Menu_GetIcon();

#endif