#include <stdio.h>
#include <stdlib.h>
#include "piece.h"

Piece* deepCopyPiece(Piece* originalPiece)
{
    if(originalPiece == NULL) { return NULL; }
    Piece* newPiece = (Piece*)malloc(sizeof(Piece));
    newPiece->id = originalPiece->id;
    newPiece->piece = originalPiece->piece;
    // newPiece->moves = 0;
    newPiece->moves = originalPiece->moves;
    newPiece->textureRect = originalPiece->textureRect;

    return newPiece;
}

// Piece* createPiece(ChessPiece pieceType)
// {
//     Piece* newPiece = (Piece*)malloc(sizeof(Piece));
//     newPiece->id = getID();
//     newPiece->piece = pieceType;
//     newPiece->moves = 0;
//     newPiece->textureRect = getTex
// }

void incrementPieceMoveCount(Piece* piece)
{
   if(piece != NULL) { piece->moves += 1; } 
}

void incrementPieceMoveCountByValue(Piece* piece, uint32_t value)
{
    if(piece != NULL) { piece->moves += value; }
}

void decrementPieceMoveCount(Piece* piece)
{
    if(piece != NULL) { piece->moves -= 1; }
}

void decrementPieceMoveCountByValue(Piece* piece, uint32_t value)
{
    if(piece != NULL) { piece->moves -= value; }
}