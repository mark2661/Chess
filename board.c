#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"


Board initBoard(void)
{
    Bool b = False;
    Image image = LoadImage("./pieces_64.png");
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

    ChessPiece whiteBackRow[8] = {WHITE_CASTLE, WHITE_KNIGHT, WHITE_BISHOP, WHITE_KING, WHITE_QUEEN, WHITE_BISHOP, WHITE_KNIGHT, WHITE_CASTLE};
    ChessPiece blackBackRow[8] = {BLACK_CASTLE, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_CASTLE};
    // init pieces
    for(size_t col=0; col<8; col++)
    {
        Piece* whitePiece = getPiece(whiteBackRow[col]);
        Piece* whitePawn = getPiece(WHITE_PAWN);
        Piece* blackPiece = getPiece(blackBackRow[col]);
        Piece* blackPawn = getPiece(BLACK_PAWN);
        if(whitePiece)
        {
            GridCell* gc = (GridCell*)malloc(sizeof(GridCell));
            gc->row = 0;
            gc->col = col;
            gc->piece = whitePiece;
            board.Board[0][col] = gc;
        }
        if(whitePawn)
        {
            GridCell *gc = (GridCell *)malloc(sizeof(GridCell));
            gc->row = 1;
            gc->col = col;
            gc->piece = whitePawn;
            board.Board[1][col] = gc;
        }

        if(blackPiece)
        {
            GridCell *gc = (GridCell *)malloc(sizeof(GridCell));
            gc->row = 7;
            gc->col = col;
            gc->piece = blackPiece;
            board.Board[7][col] = gc;
        }

        if(blackPawn)
        {
            GridCell *gc = (GridCell *)malloc(sizeof(GridCell));
            gc->row = 6;
            gc->col = col;
            gc->piece = blackPawn;
            board.Board[6][col] = gc;
         }
    }

    // init empty spaces
    for(size_t row=2; row<6; row++)
    {
        for(size_t col=0; col<8; col++)
        {
            Piece *piece = (Piece *)malloc(sizeof(Piece));
            piece->piece = EMPTY;

            GridCell *gc = (GridCell *)malloc(sizeof(GridCell));
            gc->row = row;
            gc->col = col;
            gc->piece = piece;
            board.Board[row][col] = gc;
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
                    int img_x = (gc->col * GRID_CELL_WIDTH) + (GRID_CELL_WIDTH / 2 - PIECE_WIDTH / 2); // centres image in cell
                    int img_y = (gc->row * GRID_CELL_HEIGHT) + (GRID_CELL_HEIGHT / 2 - PIECE_HEIGHT / 2);
                    Vector2 pos = {img_x, img_y};
                    DrawTextureRec(board.mainTexture, gc->piece->textureRect, pos, WHITE);
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
        if(piece != NULL)
        {
            gc->piece = piece;
        }
        else
        {
            gc->piece->piece = EMPTY;
            Rectangle dummyRect = {0, 0, 0, 0};
            gc->piece->textureRect = dummyRect;
        }
    }
}

GridCell* getCellByMousePosition(Board board)
{
    //Square s = {(int)(postion.y / GRID_CELL_WIDTH), (int)(postion.x / GRID_CELL_HEIGHT)};
    Vector2 mousePos = GetMousePosition();
    int row = ((int)mousePos.y) / GRID_CELL_HEIGHT;
    int col = ((int)mousePos.x) / GRID_CELL_WIDTH;
    if (row<0 || row>=8 || col<0 || col>=8) { return NULL; }

    return board.Board[row][col];
}

GridCell* getCellByIndex(Board board, int row, int col)
{
    if (row<0 || row>=8 || col<0 || col>=8) { return NULL; }
    return board.Board[row][col];
}

Piece* getPiece(ChessPiece piece)
{
    Piece* newPiece = (Piece*)malloc(sizeof(Piece));
    switch (piece)
    {
    // WHITE PIECES
    case WHITE_CASTLE:
        newPiece->piece = WHITE_CASTLE;
        newPiece->textureRect.x = 0 * PIECE_WIDTH;
        newPiece->textureRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_KNIGHT:
        newPiece->piece = WHITE_KNIGHT;
        newPiece->textureRect.x = 1 * PIECE_WIDTH;
        newPiece->textureRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_BISHOP:
        newPiece->piece = WHITE_BISHOP;
        newPiece->textureRect.x = 2 * PIECE_WIDTH;
        newPiece->textureRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_PAWN:
        newPiece->piece = WHITE_PAWN;
        newPiece->textureRect.x = 3 * PIECE_WIDTH;
        newPiece->textureRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_KING:
        newPiece->piece = WHITE_KING;
        newPiece->textureRect.x = 4 * PIECE_WIDTH;
        newPiece->textureRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_QUEEN:
        newPiece->piece = WHITE_QUEEN;
        newPiece->textureRect.x = 5 * PIECE_WIDTH;
        newPiece->textureRect.y = 0 * PIECE_HEIGHT;
        break;

    // BLACK PIECES
    case BLACK_CASTLE:
        newPiece->piece = BLACK_CASTLE;
        newPiece->textureRect.x = 0 * PIECE_WIDTH;
        newPiece->textureRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_KNIGHT:
        newPiece->piece = BLACK_KNIGHT;
        newPiece->textureRect.x = 1 * PIECE_WIDTH;
        newPiece->textureRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_BISHOP:
        newPiece->piece = BLACK_BISHOP;
        newPiece->textureRect.x = 2 * PIECE_WIDTH;
        newPiece->textureRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_PAWN:
        newPiece->piece = BLACK_PAWN;
        newPiece->textureRect.x = 3 * PIECE_WIDTH;
        newPiece->textureRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_KING:
        newPiece->piece = BLACK_KING;
        newPiece->textureRect.x = 4 * PIECE_WIDTH;
        newPiece->textureRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_QUEEN:
        newPiece->piece = BLACK_QUEEN;
        newPiece->textureRect.x = 5 * PIECE_WIDTH;
        newPiece->textureRect.y = 1 * PIECE_HEIGHT;
        break;

    default:
        return NULL;
        break;
    }

    newPiece->textureRect.width = PIECE_WIDTH;
    newPiece->textureRect.height = PIECE_HEIGHT;
    return newPiece;
}