#pragma once
#include <stdlib.h>
#include "menuitem.h"

#define MENU_TITLE_START_HEIGHT 200
#define MENU_TEXT_START_HEIGHT 250
#define MENU_ITEM_SPACING 50

typedef struct Menu{
    MenuItem* title;
    MenuItem** menuItems;
} Menu;

Menu* createMenu(char*, char**, size_t, Player);
void freeMenu(Menu*);
