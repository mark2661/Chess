#pragma once
#include "globals.h"
#include "piece.h"

typedef struct GridCell
{
    int row;
    int col;
    Piece* piece;

}GridCell;

void freePiece(GridCell*);
void freeGridCell(GridCell*);
GridCell* deepCopyGridCell(GridCell*);
