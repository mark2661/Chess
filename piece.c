#include <stdlib.h>
#include "piece.h"

Piece* deepCopyPiece(Piece* originalPiece)
{
    if(originalPiece == NULL) { return NULL; }
    Piece* newPiece = (Piece*)malloc(sizeof(Piece));
    newPiece->id = originalPiece->id;
    newPiece->piece = originalPiece->piece;
    newPiece->textureRect = originalPiece->textureRect;

    return newPiece;
}