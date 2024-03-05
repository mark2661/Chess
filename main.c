#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "board.h"
#include "raylib.h"

typedef struct DragPiece{
    Piece piece;
    Vector2 originalPosition;
    node validCells;
    node captureCells;
} DragPiece;

void freeDragPiece(DragPiece*);
DragPiece* getDragPiece(Board*, GridCell*);
Piece* getNewPiece(DragPiece*);
DragPiece* startDragOperation(Board*);
void endDragOperation(Board*, DragPiece*);

Bool dragging = False;

// TODO: General refactor of all files
int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chezz(TM)");

    SetTargetFPS(60);

    Board board = initBoard();
    // Bool dragging = False;
    DragPiece* dragPiece = NULL;

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        
        drawBoard(board);

        // // Drag and Drop Logic
        if(IsMouseButtonDown(0) && !dragging) // 0 is LMB
        {
            dragPiece = startDragOperation(&board);
        }

        else if (IsMouseButtonUp(0) && dragging && dragPiece != NULL) 
        {
            endDragOperation(&board, dragPiece);
            dragPiece = NULL;
        }

        // TODO: add logic for piece capture

        if (dragPiece != NULL && dragging)
        {
            node cur = dragPiece->validCells;
            while(cur != NULL)
            {
                board.colourBoard[cur->gc->row][cur->gc->col] = 2; // Temp code to visually identify valid drop cells for a piece
                cur = cur->next;
            }

            cur = dragPiece->captureCells;
            while(cur != NULL)
            {
                board.colourBoard[cur->gc->row][cur->gc->col] = 3; // Temp code to visually identify valid drop cells for a piece
                cur = cur->next;
            }

            Vector2 mousePos = GetMousePosition();
            Vector2 pos = {mousePos.x - (PIECE_WIDTH/2), mousePos.y - (PIECE_HEIGHT/2)};
            DrawTextureRec(board.mainTexture, dragPiece->piece.textureRect, pos, WHITE);

        }
        // End Drag and Drop Logic

        EndDrawing();
    }

    CloseWindow();

    return 0;
}


void freeDragPiece(DragPiece* dragPiece)
{
    freeList(dragPiece->validCells);
    dragPiece->validCells = NULL;

    freeList(dragPiece->captureCells);
    dragPiece->captureCells = NULL;

    free(dragPiece);
}

DragPiece* getDragPiece(Board* board, GridCell* gc)
{
    DragPiece* dragPiece = (DragPiece*)malloc(sizeof(DragPiece));
    dragPiece->piece.id = gc->piece->id;
    dragPiece->piece.piece = gc->piece->piece;
    dragPiece->piece.textureRect = gc->piece->textureRect;
    Vector2 originalPosition = {gc->row, gc->col};
    dragPiece->originalPosition = originalPosition;

    dragPiece->validCells = getValidCells(board, gc);
    dragPiece->captureCells = getCaptureCells(board, gc);

    return dragPiece;
}

Piece* getNewPiece(DragPiece* dragPiece)
{
    if(dragPiece != NULL)
    {
        Piece *piece = (Piece *)malloc(sizeof(Piece));
        piece->id = dragPiece->piece.id;
        piece->piece = dragPiece->piece.piece;
        piece->textureRect = dragPiece->piece.textureRect;

        return piece;
    }
    return NULL;
}

DragPiece* startDragOperation(Board* board)
{
    GridCell *gc = getCellByMousePosition(board);
    if (gc != NULL && gc->piece != NULL && gc->piece->piece != EMPTY)
    {
        dragging = True;
        DragPiece* dragPiece = getDragPiece(board, gc);
        updateBoard(board, gc->row, gc->col, NULL); // TODO: change
        return dragPiece;
    }
    return NULL;
}

void endDragOperation(Board* board, DragPiece* dragPiece)
{
            dragging = False;

            Piece *piece = getNewPiece(dragPiece);

            GridCell* gc = getCellByMousePosition(board);
            if(gc != NULL && gc->piece != NULL && gc->piece->piece == EMPTY && isValidGridCell(gc, dragPiece->validCells))
            {
                updateBoard(board, gc->row, gc->col, piece);
            }
            else
            {
                gc = getCellByIndex(board, dragPiece->originalPosition.y, dragPiece->originalPosition.x);
                if(gc != NULL)
                {
                    updateBoard(board, gc->row, gc->col, piece);
                }
            }

            resetColourBoard(board);
            freeDragPiece(dragPiece);
}