#include <stdio.h>
#include <stdlib.h>
#include "gridcell.h"

void freePiece(GridCell* gc)
{
    if(gc != NULL && gc->piece != NULL)
    {
        free(gc->piece);
        gc->piece = NULL;
    }
}


void freeGridCell(GridCell* gc)
{
    if(gc == NULL) { return; }
    freePiece(gc);
    free(gc);
}

GridCell* deepCopyGridCell(GridCell* originalGC)
{
    if(originalGC == NULL) { return NULL; }
    GridCell* newGC = (GridCell*)malloc(sizeof(GridCell));
    newGC->row = originalGC->row;
    newGC->col = originalGC->col;
    Piece* copiedPiece = deepCopyPiece(originalGC->piece);
    if(copiedPiece == NULL) { return NULL; }
    newGC->piece = copiedPiece;

    return newGC;
}