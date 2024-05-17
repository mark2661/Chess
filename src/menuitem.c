#include <stdlib.h>
#include "menuitem.h"

MenuItem* createMenuItem(char* message, ChessPiece piece)
{
    if(message == NULL) { return NULL; }
    MenuItem* newMenuItem = (MenuItem*)malloc(sizeof(MenuItem));
    newMenuItem->message = message;
    newMenuItem->messageWidth = MeasureText(message, MENU_TEXT_FONT_SIZE);
    newMenuItem->messagePosition.x = (SCREEN_WIDTH/2) - (newMenuItem->messageWidth/2);
    newMenuItem->messagePosition.y = (SCREEN_HEIGHT/2);
    Rectangle bb = {.x=(newMenuItem->messagePosition.x - BOUNDING_BOX_PADDING/2),
                    .y=(newMenuItem->messagePosition.y - BOUNDING_BOX_PADDING/2),
                    .width=newMenuItem->messageWidth + (BOUNDING_BOX_PADDING),
                    .height=MENU_TEXT_FONT_SIZE + (BOUNDING_BOX_PADDING/2)
                   };
    newMenuItem->boundingBox = bb;
    newMenuItem->piece = piece;

    return newMenuItem;
}

void freeMenuItem(MenuItem* item)
{
    if(item != NULL) { free(item); }
}