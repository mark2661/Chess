#pragma once
#include "raylib.h"
#include "globals.h"
#include "gridcell.h"

typedef struct Board{
    int colourBoard[8][8];
    GridCell* Board[8][8];
    Texture2D mainTexture;
} Board;

Board initBoard(void);
void drawBoard(Board);
void updateBoard(Board, int, int, Piece*);
GridCell* getCell(Board);