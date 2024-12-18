#pragma once
#include "globals.h"


typedef struct Piece{
    uint32_t id;
    uint32_t moves;
    ChessPiece piece;
    Rectangle textureRect;
}Piece;

Piece* deepCopyPiece(Piece*);
// Piece* createPiece(ChessPiece pieceType);
void incrementPieceMoveCount(Piece*);
void incrementPieceMoveCountByValue(Piece*, uint32_t value);
void decrementPieceMoveCount(Piece*);
void decrementPieceMoveCountByValue(Piece*, uint32_t value);