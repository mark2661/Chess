#include <stdio.h>
#include <stdlib.h>
#include "board.h"


Board initBoard(void)
{
    Bool b = False;
    Image image = LoadImage("./pieces.png");
    Board board = {.colourBoard = {{0}}, .Board={NULL}};
    board.mainTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    for(size_t i=0; i<8; i++)
    {
        for(size_t j=0; j<8; j++)
        {
            if(b)
            {
                board.colourBoard[i][j] = 1;
            }
            else
            {
                board.colourBoard[i][j] = 0;
            }
            b = !b;
        }
        b = !b;
    }

    for(size_t i=0; i<8; i++)
    {
        for(size_t j=0; j<8; j++)
        {
            // TODO: work on this (initialise grid with pieces)
            if(i < 2)
            {
                // image
                Image image = LoadImage("./castle.png");

                Texture2D texture = LoadTextureFromImage(image);
                // printf("W: %d, H: %d\n", texture.width, texture.height);
                // UnloadImage(image);

                // TODO: memory management for the GridCell
                // TODO: Do I need to free textures when i'm finished -> see UnloadTexture() function

                Piece* piece = (Piece*)malloc(sizeof(Piece));
                piece->piece = CASTLE;
                // TODO: heap allocate textures instead
                // piece->texture = (Texture2D*)malloc(sizeof(Texture2D));
                // *(piece->texture) = LoadTextureFromImage(image);
                // piece->texture = &texture;
                piece->texture = texture;
                UnloadImage(image);
                // Piece piece = {.piece=CASTLE, .texture=texture};

                GridCell* gc = (GridCell*)malloc(sizeof(GridCell));
                gc->row = i;
                gc->col = j;
                gc->piece = piece;
                board.Board[i][j] = gc;
            }
            
            else{
                Piece* piece = (Piece*)malloc(sizeof(Piece));
                piece->piece = EMPTY;
                // piece->texture = NULL;

                GridCell* gc = (GridCell*)malloc(sizeof(GridCell));
                gc->row = i;
                gc->col = j;
                gc->piece = piece;
                board.Board[i][j] = gc;
            }
        }
    }

    return board;
}

void drawBoard(Board board)
{
      // Draw board background
      size_t r = 0;
      size_t c = 0;
      for (size_t i = 0; i < SCREEN_WIDTH; i += GRID_CELL_WIDTH)
      {
          for (size_t j = 0; j < SCREEN_HEIGHT; j += GRID_CELL_HEIGHT)
          {
              if (r < 8 && c < 8)
              {
                  if (board.colourBoard[r][c] == 0)
                  {
                      DrawRectangle(i, j, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, c1);
                  }
                  else
                  {
                      DrawRectangle(i, j, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, c2);
                  }
                  r += 1;
              }
          }
          c += 1;
          r = 0;
      }

      // Draw pieces
      for(size_t i=0; i<8; i++)
      {
        for(size_t j=0; j<8; j++)
        {
            if(board.Board[i][j] != NULL)
            {
                GridCell* gc = board.Board[i][j];
                if(gc->piece != NULL && gc->piece->piece != EMPTY)
                {
                    // TODO: Fix bug causing segfault when trying to access texture
                    int img_x = (gc->col * GRID_CELL_WIDTH) + (GRID_CELL_WIDTH/2 - gc->piece->texture.width/2); // centres image in cell
                    int img_y = (gc->row * GRID_CELL_HEIGHT) + (GRID_CELL_HEIGHT/2 - gc->piece->texture.height/2);
                    // printf("X: %d, Y: %d\n", img_x, img_y);
                    // printf("W: %d, H: %d\n", gc->piece->texture->width, gc->piece->texture->height);
                    // DrawTexture(*(gc->piece->texture), img_x, img_y, WHITE);
                    DrawTexture(gc->piece->texture, img_x, img_y, WHITE);
                }
            }
        }
      }
}

void updateBoard(Board board, int row, int col, Piece* piece)
{
    if(row>=0 && row<8 && col>=0 && col<8)
    {
        GridCell* gc = board.Board[row][col];
        gc->piece = piece;
    }
}

GridCell* getCell(Board board)
{
    //Square s = {(int)(postion.y / GRID_CELL_WIDTH), (int)(postion.x / GRID_CELL_HEIGHT)};
    Vector2 mousePos = GetMousePosition();
    int row = ((int)mousePos.y) / GRID_CELL_HEIGHT;
    int col = ((int)mousePos.x) / GRID_CELL_WIDTH;
    if (row<0 || row>=8 || col<0 || col>=8) { return NULL; }

    return board.Board[row][col];
}