#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "globals.h"
#include "gridcell.h"

#define SIZE 100

struct LinkedList{
    GridCell* gc;
    struct LinkedList* next;
};

typedef struct HashNode{
    Piece* piece;
    struct HashNode* next;
} HashNode;

typedef struct HashSet{
    HashNode* buckets[SIZE];
} HashSet;

typedef struct LinkedList* node;

typedef struct Board{
    int colourBoard[8][8];
    GridCell* Board[8][8];
    Texture2D mainTexture;
    HashSet *pawnSet;
} Board;


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

HashNode* createHashNode(Piece*);
HashSet* createHashSet();
int hash(int);
void insert(HashSet*, Piece*);
bool contains(HashSet*, Piece*);



