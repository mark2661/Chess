#pragma once
#include <stdint.h>
#include "raylib.h"

#define EN_PASSANT_IDTENTIFIER_THRESHOLD 100
#define EN_PASSANT_DISABLED_THRESHOLD 1000

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

typedef enum Player
{
    PLAYER_SPECTATOR,
    PLAYER_WHITE,
    PLAYER_BLACK,
} Player;

typedef enum GameState{
    WHITE_IN_PLAY,
    BLACK_IN_PLAY,
    WHITE_IN_CHECK,
    BLACK_IN_CHECK,
    WHITE_PIECE_SELECT_MENU,
    BLACK_PIECE_SELECT_MENU,
    GAME_OVER
} GameState;

typedef struct Pair{
    int x;
    int y;
} Pair;


uint32_t getID();
extern const int GRID_CELL_WIDTH;
extern const int GRID_CELL_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int PIECE_WIDTH;
extern const int PIECE_HEIGHT;

extern Color c1;  // greenish colour
extern Color c2; // white colour