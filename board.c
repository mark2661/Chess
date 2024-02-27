#include <stdio.h>
#include "board.h"


Board initBoard(void)
{
    Bool b = False;
    Board board = {.colourBoard = {{0}}, .Board = {{0}}};
    for(size_t i=0; i<8; i++)
    {
        for(size_t j=0; j<8; j++)
        {
            if(b)
            {
                board.Board[i][j] = 1;
            }
            else
            {
                board.Board[i][j] = 0;
            }
            b = !b;
        }
        b = !b;
    }

    return board;
}

void drawBoard(Board board)
{
      size_t r = 0;
        size_t c = 0;
        for(size_t i=0; i<SCREEN_WIDTH; i+= GRID_CELL_WIDTH)
        {
            for(size_t j=0; j<SCREEN_HEIGHT; j+= GRID_CELL_HEIGHT)
            {
                if (r < 8 && c < 8)
                {
                    if(board.Board[r][c] == 0)
                    {
                        DrawRectangle(i, j , GRID_CELL_WIDTH, GRID_CELL_HEIGHT, c1);
                    }
                    else
                    {
                        DrawRectangle(i, j , GRID_CELL_WIDTH, GRID_CELL_HEIGHT, c2);
                    }
                    r += 1;
                }
           }
           c += 1;
           r = 0;
        }
}
