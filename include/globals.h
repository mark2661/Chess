#pragma once
#include <stdint.h>
#include "raylib.h"

#define EN_PASSANT_IDENTIFIER_THRESHOLD 100
#define EN_PASSANT_DISABLED_THRESHOLD 1000

// ASSET FILE PATHS
#define SOUND_DIR_PATH "../assets/sounds/"
#define FONTS_DIR_PATH "../assets/fonts/"
#define IMAGES_DIR_PATH "../assets/images/"
#define CONFIG_DIR_PATH "../config/"
#define DATA_DIR_PATH "../data/"
#define ROOT_DIR_PATH "../"
#define CHESS_MOVE_SOUND_FILE_PATH SOUND_DIR_PATH "chess_move.mp3"
#define CHESS_CAPTURE_SOUND_FILE_PATH SOUND_DIR_PATH "chess_capture.mp3"
#define CHESS_ILLEGAL_SOUND_FILE_PATH SOUND_DIR_PATH "chess_illegal.mp3"
#define CHESS_PROMOTE_SOUND_FILE_PATH SOUND_DIR_PATH "chess_promote.mp3"
#define CHESS_CASTLE_SOUND_FILE_PATH SOUND_DIR_PATH "chess_castle.mp3"
#define CHESS_CHECK_SOUND_FILE_PATH SOUND_DIR_PATH "chess_check.mp3"
#define CHESS_GAME_OVER_SOUND_FILE_PATH SOUND_DIR_PATH "chess_game_over.mp3"
#define CHESS_PIECES_TEXTURE_FILE_PATH IMAGES_DIR_PATH "pieces_64.png"

//char SOUND_FILE_PATH[256];
//snprintf(SOUND_FILE_PATH, sizeof(SOUND_FILE_PATH), "%s%s", SOUND_DIR_PATH, "chess_move.mp3");
//moveSound = LoadSound(SOUND_FILE_PATH);
//snprintf(SOUND_FILE_PATH, sizeof(SOUND_FILE_PATH), "%s%s", SOUND_DIR_PATH, "chess_capture.mp3");
//captureSound = LoadSound(SOUND_FILE_PATH);
//snprintf(SOUND_FILE_PATH, sizeof(SOUND_FILE_PATH), "%s%s", SOUND_DIR_PATH, "chess_illegal.mp3");
//illegalMoveSound = LoadSound(SOUND_FILE_PATH);
//snprintf(SOUND_FILE_PATH, sizeof(SOUND_FILE_PATH), "%s%s", SOUND_DIR_PATH, "chess_promote.mp3");
//pawnPromotionSound = LoadSound(SOUND_FILE_PATH);
//snprintf(SOUND_FILE_PATH, sizeof(SOUND_FILE_PATH), "%s%s", SOUND_DIR_PATH, "chess_castle.mp3");
//castleSound = LoadSound(SOUND_FILE_PATH);
//snprintf(SOUND_FILE_PATH, sizeof(SOUND_FILE_PATH), "%s%s", SOUND_DIR_PATH, "chess_check.mp3");
//checkSound = LoadSound(SOUND_FILE_PATH);
//snprintf(SOUND_FILE_PATH, sizeof(SOUND_FILE_PATH), "%s%s", SOUND_DIR_PATH, "chess_game_over.mp3");
//gameOverSound = LoadSound(SOUND_FILE_PATH);

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
    WHITE_PIECE_SELECT_MENU,
    BLACK_PIECE_SELECT_MENU,
    GAME_OVER,
    RESTART,
    QUIT
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