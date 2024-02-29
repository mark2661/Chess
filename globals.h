#pragma once
#include "raylib.h"

typedef enum Bool{
    False,
    True
} Bool;

typedef enum ChessPiece{
    EMPTY,
    PAWN,
    CASTLE,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
} ChessPiece;

extern const int GRID_CELL_WIDTH;
extern const int GRID_CELL_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern Color c1;  // greenish colour
extern Color c2; // white colour