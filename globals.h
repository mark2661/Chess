#pragma once
#include "raylib.h"

typedef enum Bool{
    False,
    True
} Bool;

typedef enum ChessPiece
{
    EMPTY,
    WHITE_PAWN,
    WHITE_CASTLE,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_CASTLE,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_QUEEN,
    BLACK_KING
} ChessPiece;

typedef struct Pair{
    int x;
    int y;
} Pair;

extern const int GRID_CELL_WIDTH;
extern const int GRID_CELL_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int PIECE_WIDTH;
extern const int PIECE_HEIGHT;

extern Color c1;  // greenish colour
extern Color c2; // white colour