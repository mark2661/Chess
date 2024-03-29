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

// typedef struct EnPassantContainer
// {
// } EnPassantPair;


Board initBoard(void);
void drawBoard(Board);
void updateBoard(Board*, int, int, Piece*);
void resetColourBoard(Board*);
GridCell* getCellByMousePosition(Board*);
GridCell* getCellByIndex(Board*,int,int);
GridCell* getCellContainingPiece(Board*, ChessPiece);
GridCellContainer* getAllCellsContainingPiece(Board*, ChessPiece);
size_t countNumberOfPieceTypeOnBoard(Board*, ChessPiece);
Piece* getPiece(ChessPiece);
node getValidCells(Board*, GridCell*);
node getLineOfSightCells(Board*, GridCell*);
node getCastlingCells(Board*, GridCell*);
void performCastle(Board*, GridCell*);
void performKingSideCastle(Board*, GridCell*);
void performQueenSideCastle(Board*, GridCell*);
node getCaptureCells(Board*, GridCell*);
node getCaptureCellsIgnoreCheck(Board*, GridCell*);
node getEnPassantCells(Board*, GridCell*);
node getEnPassantNeighours(Board*, GridCell*);
Bool isWhitePiece(Piece*);
Bool isBlackPiece(Piece*);
Bool isAllowedToCastle(Board*, GridCell*);
Bool isAllowedToCastleQueenSide(Board*, GridCell*);
Bool isAllowedToCastleKingSide(Board*, GridCell*);
Rectangle getTextureRect(ChessPiece);
Bool isInCheck(Board*, Player);
Bool testCheck(Board*, GridCell*, GridCell*, Player);
Board* deepCopyBoard(Board*);
void freeBoard(Board*);

// Linked List
node createNode();
node addNode(node, GridCell*);
node concatenate_LL(node, node);
void freeList(node);
Bool contains_LL(node, GridCell*);
node remove_LL(node, GridCell*);
Bool isValidGridCell(GridCell*, node);

// Hash Set
HashNode* createHashNode(Piece*);
HashSet* createHashSet();
HashSet* copyHashSet(HashSet*);
void freeHashSet(HashSet*);
int hash(int);
void insert(HashSet*, Piece*);
bool contains(HashSet*, Piece*);


// Debug functions
void printBoard(Board*, char*);
void printLL(node);
void printHashSet(HashSet*);

