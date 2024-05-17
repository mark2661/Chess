#pragma once
#include "globals.h"
#include "raylib.h"

#define BOUNDING_BOX_PADDING 5
#define MENU_TEXT_FONT_SIZE 42

typedef struct MenuItem{
    const char* message;
    Vector2 messagePosition;
    int messageWidth;
    Rectangle boundingBox;
    ChessPiece piece;
} MenuItem;

MenuItem* createMenuItem(char*, ChessPiece);
void freeMenuItem(MenuItem*);