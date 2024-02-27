#include <stdio.h>
#include "globals.h"
#include "board.h"
#include "raylib.h"


typedef struct Square
{
    int row;
    int col
} Square;

Square getSquare(Vector2);
Bool cursorOnImage(Vector2, Vector2);

int main(void)
{
    Bool b = False;
    Bool dragging = False;

    // create board
    // int board[8][8] = {{0}};
    // for(size_t i=0; i<8; i++)
    // {
    //     for(size_t j=0; j<8; j++)
    //     {
    //         if(b)
    //         {
    //             board[i][j] = 1;
    //         }
    //         else
    //         {
    //             board[i][j] = 0;
    //         }
    //         b = !b;
    //     }
    //     b = !b;
    // }

    // print board
    // for (size_t i = 0; i < 8; i++)
    // {
    //     printf("[");
    //     for(size_t j=0; j<8; j++)
    //     {
    //        printf(" %d", board[i][j]); 
    //     }
    //     printf(" ]\n");
    // }


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chezz(TM)");

    // image
    Image image = LoadImage("./castle.png");

    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    int img_x = 0;
    int img_y = 0;
    Board board = initBoard();

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // size_t r = 0;
        // size_t c = 0;
        // for(size_t i=0; i<screenWidth; i+=w)
        // {
        //     for(size_t j=0; j<screenHeight; j+=h)
        //     {
        //         if (r < 8 && c < 8)
        //         {
        //             if(board[r][c] == 0)
        //             {
        //                 DrawRectangle(i, j , w, h, c1);
        //             }
        //             else
        //             {
        //                 DrawRectangle(i, j , w, h, c2);
        //             }
        //             r += 1;
        //         }
        //    }
        //    c += 1;
        //    r = 0;
        // }
        // drawBoard();

        // // Drag and Drop Logic
        // Vector2 texturePos = {(float)img_x, (float)img_y}; 
        // // 0 is LMB
        // if(IsMouseButtonDown(0) && cursorOnImage(GetMousePosition(), texturePos)) { dragging = True; }

        // else if (IsMouseButtonUp(0)) { dragging = False; }

        // if (dragging)
        // {
        //     // places centre of image on the mouse cursor
        //     Vector2 mousePos = GetMousePosition();
        //     img_x = mousePos.x - (texture.width/2);
        //     img_y = mousePos.y - (texture.height/2);
        // }
        // else
        // {
        //     // snaps image to the board cell that the cursor is on
        //     Vector2 texturePos = {img_x + (texture.width/2), img_y + (texture.height/2)};
        //     Square s = getSquare(texturePos);
        //     img_x = (s.col * GRID_CELL_WIDTH) + (SCREEN_WIDTH/2 - texture.width/2); // centres the image in the cell
        //     img_y = (s.row * GRID_CELL_HEIGHT) + (SCREEN_HEIGHT/2 - texture.height/2);
        // }
        // // End Drag and Drop Logic

        // DrawTexture(texture, img_x , img_y, WHITE);
        drawBoard(board);
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

Bool cursorOnImage(Vector2 mousePosition, Vector2 imagePosition)
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

void createBoard()
{
    Color c1 = {118, 150, 86, 255};  // greenish colour
    Color c2 = {255, 255, 255, 255}; // white colour
    Bool b = False;
    const int h = 200;
    const int w = 200;

}

