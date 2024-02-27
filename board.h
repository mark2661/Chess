#pragma once
#include "raylib.h"
#include "globals.h"


typedef struct Board{
    int colourBoard[8][8];
    int Board[8][8];
} Board;

Board initBoard(void);
void drawBoard(Board);
Bool updateBoard(int row, int col);