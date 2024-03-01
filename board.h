#pragma once
#include "raylib.h"
#include "globals.h"
#include "gridcell.h"

typedef struct Board{
    int colourBoard[8][8];
    GridCell* Board[8][8];
    Texture2D mainTexture;
} Board;

struct LinkedList{
    GridCell* gc;
    struct LinkedList* next;

};

typedef struct LinkedList* node;

Board initBoard(void);
void drawBoard(Board);
void updateBoard(Board, int, int, Piece*);
void resetColourBoard(Board*);
GridCell* getCellByMousePosition(Board);
GridCell* getCellByIndex(Board*,int,int);
Piece* getPiece(ChessPiece);
node getValidCells(Board*, GridCell*);
node createNode();
node addNode(node, GridCell*);
void freeList(node);
Bool isValidGridCell(GridCell*, node);


