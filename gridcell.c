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