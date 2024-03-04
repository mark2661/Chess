#pragma once
#include "globals.h"


typedef struct Piece{
    uint32_t id;
    ChessPiece piece;
    // Texture2D texture;
    Rectangle textureRect;
}Piece;