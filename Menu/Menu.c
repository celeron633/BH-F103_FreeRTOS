#include "Menu.h"

static Menu *currentMenu;

void Menu_Init(Menu *menu)
{
    menu->name = NULL;
    menu->icon = NULL;
    menu->function = NULL;
    menu->father = NULL;
    menu->child = NULL;
    menu->prev = NULL;
    menu->next = NULL;
}

void Menu_SetName(Menu *menu, const char *name)
{
    menu->name = name;
}

void Menu_SetIcon(Menu *menu, const uint8_t *ico)
{
    menu->icon = ico;
}

void Menu_SetFunction(Menu *menu, MenuFunction func)
{
    menu->function = func;
}

void Menu_SetFather(Menu *menu, Menu *father)
{
    menu->father = father;

    // 如果father节点没有child节点, 则father节点的child节点直接指向要设置父节点的menu对象
    if (father->child == NULL) {
        father->child = menu;
    } else {
        // 否则将menu加入到father子节点的最后去
        Menu *tmp = father->child;
        while (tmp->next != NULL)
        {
            /* code */
            tmp = tmp->next;
        }
        Menu_SetNext(tmp, menu);
        /*
        tmp->next = menu;
        menu->prev = tmp;
        */
    }
}

void Menu_SetChild(Menu *menu, Menu *childMenu)
{
    childMenu->father = menu;

    // 要设置child节点的child指针为空, 没有子节点
    if (menu->child == NULL) {
        // 则child指向child对象
        menu->child = childMenu;
    } else { // 如果已经有子节点了, 需要判断childMenu是不是其子节点的前后
        Menu *tmp = childMenu;
        // 向前遍历
        while (tmp->prev != NULL)
        {
            tmp = tmp->prev;
            // 一旦menu的child属于childMenu的兄弟姐妹, 则menu的child重新指向childMenu
            if (menu->child == tmp) {
                menu->child = childMenu;
                return;
            }
        }
        // 再向后遍历
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
            // 一旦menu的child属于childMenu的兄弟姐妹, 则menu的child重新指向childMenu
            if (menu->child == tmp) {
                menu->child = childMenu;
                return;
            }
        }
    }
}

void Menu_SetNext(Menu *menu, Menu *nextMenu)
{
    menu->next = nextMenu;
    nextMenu->prev = menu;
}

void Menu_SetPrev(Menu *menu, Menu *prevMenu)
{
    menu->prev = prevMenu;
    prevMenu->next = menu;
}

// for currentMenu
int Menu_ToFather()
{
    if (currentMenu->father != NULL) {
        currentMenu = currentMenu->father;
        return 1;
    }
    return 0;
}

int Menu_ToChild()
{
    if (currentMenu->child != NULL) {
        currentMenu = currentMenu->child;
        return 1;
    }
    return 0;
}

int Menu_ToNext()
{
    if (currentMenu->next != NULL) {
        currentMenu = currentMenu->next;
        return 1;
    }
    return 0;
}

int Menu_ToPrev()
{
    if (currentMenu->prev != NULL) {
        currentMenu = currentMenu->prev;
        return 1;
    }
    return 0;
}

void Menu_SetCurrent(Menu *mu)
{
    currentMenu = mu;
}

Menu * Menu_GetCurrent()
{
    return currentMenu;
}

Menu * Menu_GetFather()
{
    return currentMenu->father;
}

Menu * Menu_GetChild()
{
    return currentMenu->child;
}

Menu * Menu_GetPrev()
{
    return currentMenu->prev;
}

Menu * Menu_GetNext()
{
    return currentMenu->next;
}

const char * Menu_GetName()
{
    return currentMenu->name;
}

const uint8_t * Menu_GetIcon()
{
    return currentMenu->icon;
}