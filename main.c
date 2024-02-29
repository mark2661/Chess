#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "board.h"
#include "raylib.h"


typedef struct Square
{
    int row;
    int col
} Square;

Square getSquare(Vector2);
Bool cursorOnImage(Board, Vector2, Vector2);

int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chezz(TM)");

    // image
    // Image image = LoadImage("./castle.png");

    // Texture2D texture = LoadTextureFromImage(image);
    // UnloadImage(image);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    int img_x = 0;
    int img_y = 0;
    Board board = initBoard();
    Bool dragging = False;
    // Texture2D* dragTexture = NULL;
    Texture2D dragTexture;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        
        drawBoard(board);

        // // Drag and Drop Logic
        // Vector2 texturePos = {(float)img_x, (float)img_y}; 
        // // 0 is LMB
        if(IsMouseButtonDown(0) && !dragging) 
        { 
            GridCell* gc = getCell(board);
            if(gc != NULL && gc->piece != NULL && gc->piece->piece != EMPTY)
            {
                dragging = True; 
                // TODO: Destroy texture and malloc a new one here
                // dragTexture = &(gc->piece->texture);
                dragTexture = gc->piece->texture;
                updateBoard(board, gc->row, gc->col, NULL); // TODO: change
            }
        }

        else if (IsMouseButtonUp(0) && dragging) 
        { 
            //TODO: need to do proper memory management
            dragging = False; 

            Piece* piece = (Piece*)malloc(sizeof(Piece));
            piece->piece = CASTLE; // TODO: need to change this
            piece->texture = dragTexture; 

            GridCell* gc = getCell(board);
            if(gc != NULL)
            {
                updateBoard(board, gc->row, gc->col, piece);
            }

            // dragTexture = NULL;
        }

        if (dragging)
        {
            // places centre of image on the mouse cursor
            Vector2 mousePos = GetMousePosition();
            // img_x = mousePos.x - (dragTexture->width/2);
            // img_y = mousePos.y - (dragTexture->height/2);
            // DrawTexture(*(dragTexture), img_x, img_y, WHITE);
            img_x = mousePos.x - (dragTexture.width / 2);
            img_y = mousePos.y - (dragTexture.height/2);
            DrawTexture(dragTexture, img_x, img_y, WHITE);

        }
        // else
        // {
        //     // snaps image to the board cell that the cursor is on
        //     Vector2 texturePos = {img_x + (texture.width/2), img_y + (texture.height/2)};
        //     Square s = getSquare(texturePos);
        //     img_x = (s.col * GRID_CELL_WIDTH) + (SCREEN_WIDTH/2 - texture.width/2); // centres the image in the cell
        //     img_y = (s.row * GRID_CELL_HEIGHT) + (SCREEN_HEIGHT/2 - texture.height/2);
        // }
        // End Drag and Drop Logic

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

Square getSquare(Vector2 postion)
{
    // int w = 100;
    // int h = 100;
    Square s = {(int)(postion.y / GRID_CELL_WIDTH), (int)(postion.x / GRID_CELL_HEIGHT)};
    return s;
}

Bool cursorOnImage(Board board, Vector2 mousePosition, Vector2 imagePosition)
{
    // GetMousePosition returns {0.0, 0.0} for some reason when the cursor is not on the window
    // This was causing the programme to think that the cursor was on the top left square when it was not
    if(mousePosition.x > 0 && mousePosition.y > 0)
    {
        Square mouseSq = getSquare(mousePosition);
        Square imgSq = getSquare(imagePosition);

        return (mouseSq.row == imgSq.row && mouseSq.col == imgSq.col);
    }

    return False;
}

