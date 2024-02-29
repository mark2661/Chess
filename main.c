#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "board.h"
#include "raylib.h"

int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chezz(TM)");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    int img_x = 0;
    int img_y = 0;
    Board board = initBoard();
    Bool dragging = False;
    Piece dragPiece = {.piece=EMPTY, .textureRect={0,0,0,0}};
    Vector2 dragPieceOriginalPos;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        
        drawBoard(board);

        // // Drag and Drop Logic
        if(IsMouseButtonDown(0) && !dragging) // 0 is LMB
        { 
            GridCell* gc = getCellByMousePosition(board);
            if(gc != NULL && gc->piece != NULL && gc->piece->piece != EMPTY)
            {
                dragging = True; 
                dragPiece.piece = gc->piece->piece;
                dragPiece.textureRect = gc->piece->textureRect;
                dragPieceOriginalPos.y = gc->row;
                dragPieceOriginalPos.x = gc->col;
                updateBoard(board, gc->row, gc->col, NULL); // TODO: change
            }
        }

        else if (IsMouseButtonUp(0) && dragging) 
        { 
            //TODO: need to do proper memory management
            dragging = False; 

            Piece* piece = (Piece*)malloc(sizeof(Piece));
            piece->piece = dragPiece.piece; // TODO: need to change this
            piece->textureRect = dragPiece.textureRect;

            GridCell* gc = getCellByMousePosition(board);
            if(gc != NULL && gc->piece != NULL && gc->piece->piece == EMPTY)
            {
                updateBoard(board, gc->row, gc->col, piece);
            }
            // if the selected cell is occupied place the piece back in its origin cell
            else
            {
                gc = getCellByIndex(board, dragPieceOriginalPos.y, dragPieceOriginalPos.x);
                if(gc != NULL)
                {
                    updateBoard(board, gc->row, gc->col, piece);
                }
            }

            dragPiece.piece = EMPTY;
            Rectangle dummyRect =  {0, 0, 0, 0};
            dragPiece.textureRect = dummyRect;
            // TODO: reset dragPieceOriginalPos variable

        }

        if (dragging)
        {
            // places centre of image on the mouse cursor
            Vector2 mousePos = GetMousePosition();
            img_x = mousePos.x - (PIECE_WIDTH/2);
            img_y = mousePos.y - (PIECE_HEIGHT/2);
            Vector2 pos = {img_x, img_y};
            DrawTextureRec(board.mainTexture, dragPiece.textureRect, pos, WHITE);

        }
        // End Drag and Drop Logic

        EndDrawing();
    }

    CloseWindow();

    return 0;
}


