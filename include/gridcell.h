#pragma once
#include "globals.h"
#include "piece.h"

typedef struct GridCell
{
    int row;
    int col;
    Piece* piece;

}GridCell;

// TODO: create a destructor function to clean up memory
typedef struct GridCellContainer
{
    GridCell** cellsArray;
    size_t size;
} GridCellContainer;

void freePiece(GridCell*);
void freeGridCell(GridCell*);
GridCell* deepCopyGridCell(GridCell*);
