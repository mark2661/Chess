#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "board.h"
#include "raylib.h"

#define BOUNDING_BOX_PADDING 5

typedef struct DragPiece{
    Piece piece;
    Vector2 originalPosition;
    node validCells;
    node captureCells;
} DragPiece;

void freeDragPiece(DragPiece*);
DragPiece* getDragPiece(Board*, GridCell*);
Piece* getNewPiece(DragPiece*);
DragPiece* startDragOperation(Board*, Player);
void endDragOperation(Board*, DragPiece*);
void gameIteration(Board*, Player);
void pieceSelectMenuIteration(Board*, Player);

Bool dragging = False;
// GameState state = WHITE_IN_PLAY;
GameState state = WHITE_PIECE_SELECT_MENU;

// TODO: Add state machine pattern
int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chezz(TM)");

    SetTargetFPS(60);

    Board board = initBoard();

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBoard(board);

        switch (state)
        {
        case WHITE_IN_PLAY:
            gameIteration(&board, PLAYER_WHITE);
            break;
        case BLACK_IN_PLAY: 
            gameIteration(&board, PLAYER_BLACK);
            break;
        // TODO: handle check conditions
        case WHITE_IN_CHECK:
            break;
        case BLACK_IN_CHECK:
            break;
        case WHITE_PIECE_SELECT_MENU:
            pieceSelectMenuIteration(&board, PLAYER_WHITE);
            break;
        case BLACK_PIECE_SELECT_MENU:
            break;
        default:
            break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void gameIteration(Board* board, Player player)
{

    static DragPiece* dragPiece = NULL;
    // // Drag and Drop Logic
    if (IsMouseButtonDown(0) && !dragging) // 0 is LMB
    {
        dragPiece = startDragOperation(board, player);
    }

    else if (IsMouseButtonUp(0) && dragging && dragPiece != NULL)
    {
        endDragOperation(board, dragPiece);
        dragPiece = NULL;
    }

    if (dragPiece != NULL && dragging)
    {
        node cur = dragPiece->validCells;
        while (cur != NULL)
        {
            board->colourBoard[cur->gc->row][cur->gc->col] = 2; // Temp code to visually identify valid drop cells for a piece
            cur = cur->next;
        }

        cur = dragPiece->captureCells;
        while (cur != NULL)
        {
            board->colourBoard[cur->gc->row][cur->gc->col] = 3; // Temp code to visually identify valid drop cells for a piece
            cur = cur->next;
        }

        Vector2 mousePos = GetMousePosition();
        Vector2 pos = {mousePos.x - (PIECE_WIDTH / 2), mousePos.y - (PIECE_HEIGHT / 2)};
        DrawTextureRec(board->mainTexture, dragPiece->piece.textureRect, pos, WHITE);
    }
    // End Drag and Drop Logic
}

void pieceSelectMenuIteration(Board* board, Player player)
{
   char* message = "Sample Text";
   int messageWidth = MeasureText(message, 42);
   int centre_x = (SCREEN_WIDTH / 2) - (messageWidth / 2); 
   int centre_y = SCREEN_HEIGHT / 2; 
   Rectangle boundingBox = {.x=(centre_x - BOUNDING_BOX_PADDING/2),
                            .y=(centre_y - BOUNDING_BOX_PADDING/2), 
                            .width = messageWidth+BOUNDING_BOX_PADDING/2, 
                            .height = 42+BOUNDING_BOX_PADDING/2
                           };
   DrawText(message, centre_x, centre_y, 42, BLACK);
   // Draw bounding box if mousing over text
   if(CheckCollisionPointRec(GetMousePosition(), boundingBox))
   {
       DrawRectangleLinesEx(boundingBox, 3.0f, RED);
   }
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

DragPiece* startDragOperation(Board* board, Player player)
{
    GridCell *gc = getCellByMousePosition(board);
    if (gc != NULL && gc->piece != NULL && gc->piece->piece != EMPTY)
    {
        if ((player == PLAYER_WHITE && isWhitePiece(gc->piece)) || (player == PLAYER_BLACK && isBlackPiece(gc->piece)))
        {
            dragging = True;
            DragPiece *dragPiece = getDragPiece(board, gc);
            updateBoard(board, gc->row, gc->col, NULL); // TODO: change
            return dragPiece;
        }
    }
    return NULL;
}

void endDragOperation(Board* board, DragPiece* dragPiece)
{
            dragging = False;

            Piece *piece = getNewPiece(dragPiece);

            GridCell* gc = getCellByMousePosition(board);
            // Move to empty cell
            if(gc != NULL && gc->piece != NULL && gc->piece->piece == EMPTY && isValidGridCell(gc, dragPiece->validCells))
            {
                updateBoard(board, gc->row, gc->col, piece);
                if(state == WHITE_IN_PLAY) { state = BLACK_IN_PLAY; }
                else if(state == BLACK_IN_PLAY) { state = WHITE_IN_PLAY; }
            }
            // Capture piece and move to cell
            else if(gc != NULL && gc->piece != NULL && gc->piece->piece != EMPTY && isValidGridCell(gc, dragPiece->captureCells))
            {
                // remove captured piece
                freePiece(gc);
                updateBoard(board, gc->row, gc->col, piece);
                if(state == WHITE_IN_PLAY) { state = BLACK_IN_PLAY; }
                else if(state == BLACK_IN_PLAY) { state = WHITE_IN_PLAY; }
            }
            // Invalid move return to origin cell
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