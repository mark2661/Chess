#include <stdio.h>
#include "menu.h"
#include "globals.h"


Menu* createMenu(char* title, char** options, size_t length, Player player)
{
    if(title == NULL) { return NULL; }
    int item_y = MENU_TEXT_START_HEIGHT;
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    menu->title = createMenuItem(title, EMPTY);
    menu->menuItems = (MenuItem**)malloc(sizeof(MenuItem*)*length);
    for(size_t i=0; i<length; i++)
    {
        char* option = options[i];
        if(player == PLAYER_WHITE)
        {
            menu->menuItems[i] = createMenuItem(option, WHITE_CASTLE + i);
        }
        else
        {
            menu->menuItems[i] = createMenuItem(option, BLACK_CASTLE + i);
        }

        menu->menuItems[i]->messagePosition.y = item_y;
        menu->menuItems[i]->boundingBox.y = item_y - (BOUNDING_BOX_PADDING/2);
        item_y += MENU_ITEM_SPACING;
    }

    return menu;
}