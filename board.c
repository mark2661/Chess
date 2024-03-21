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
    board.pawnSet = createHashSet(); 
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

    ChessPiece whiteBackRow[8] = {WHITE_CASTLE, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_CASTLE};
    ChessPiece blackBackRow[8] = {BLACK_CASTLE, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_CASTLE};
    // init pieces
    for(size_t col=0; col<8; col++)
    {
        Piece* whitePiece = getPiece(whiteBackRow[col]);
        Piece* whitePawn = getPiece(WHITE_PAWN);
        Piece* blackPiece = getPiece(blackBackRow[col]);
        Piece* blackPawn = getPiece(BLACK_PAWN);
        // if(whitePiece)
        if(blackPiece)
        {
            GridCell* gc = (GridCell*)malloc(sizeof(GridCell));
            gc->row = 0;
            gc->col = col;
            // gc->piece = whitePiece;
            gc->piece = blackPiece;
            board.Board[0][col] = gc;
        }
        // if(whitePawn)
        if(blackPawn)
        {
            GridCell *gc = (GridCell *)malloc(sizeof(GridCell));
            gc->row = 1;
            gc->col = col;
            // gc->piece = whitePawn;
            gc->piece = blackPawn;
            board.Board[1][col] = gc;
        }

        // if(blackPiece)
        if(whitePiece)
        {
            GridCell *gc = (GridCell *)malloc(sizeof(GridCell));
            gc->row = 7;
            gc->col = col;
            // gc->piece = blackPiece;
            gc->piece = whitePiece;
            board.Board[7][col] = gc;
        }

        // if(blackPawn)
        if(whitePawn)
        {
            GridCell *gc = (GridCell *)malloc(sizeof(GridCell));
            gc->row = 6;
            gc->col = col;
            // gc->piece = blackPawn;
            gc->piece = whitePawn;
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
                  else if(board.colourBoard[r][c] == 1)
                  {
                      DrawRectangle(i, j, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, c2);
                  }

                  // Temp code to visually identify valid drop cells for a piece
                  else if(board.colourBoard[r][c] == 2)
                  {
                      DrawRectangle(i, j, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, YELLOW);
                  }
                  else if (board.colourBoard[r][c] == 3)
                  {
                      DrawRectangle(i, j, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, RED);
                  }

                  else if(board.colourBoard[r][c] == 4)
                  {
                      DrawRectangle(i, j, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, ORANGE);
                  }
                  r += 1;
              }
          }
          c += 1;
          r = 0;
      }

      // Draw grid lines
      // vertical lines
      for (size_t x = 0; x < SCREEN_WIDTH; x += GRID_CELL_WIDTH)
      {
          Vector2 start = {x, 0};
          Vector2 end = {x, SCREEN_HEIGHT};
          DrawLineEx(start, end, 1.0f, BLACK);
      }
      // horizontal lines
      for (size_t y=0; y<SCREEN_HEIGHT; y+=GRID_CELL_HEIGHT)
      {
          Vector2 start = {0, y};
          Vector2 end = {SCREEN_WIDTH, y};
          DrawLineEx(start, end, 1.0f, BLACK);
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

void updateBoard(Board* board, int row, int col, Piece* piece)
{
    if(row>=0 && row<8 && col>=0 && col<8)
    {
        GridCell* gc = board->Board[row][col];
        if(piece != NULL)
        {
            // free(gc->piece);
            // gc->piece = piece;
            gc->piece = deepCopyPiece(piece);
        }
        else
        {
            gc->piece = getPiece(EMPTY);
            // gc->piece->piece = EMPTY;
            // gc->piece->textureRect = getTextureRect(gc->piece->piece);
        }
    }
}

void resetColourBoard(Board* board)
{
    Bool b = False;
    for(size_t i=0; i<8; i++)
    {
        for(size_t j=0; j<8; j++)
        {
            if(b)
            {
                board->colourBoard[i][j] = 1;
            }
            else
            {
                board->colourBoard[i][j] = 0;
            }
            b = !b;
        }
        b = !b;
    }
}

GridCell* getCellByMousePosition(Board* board)
{
    Vector2 mousePos = GetMousePosition();
    int row = ((int)mousePos.y) / GRID_CELL_HEIGHT;
    int col = ((int)mousePos.x) / GRID_CELL_WIDTH;
    if (row<0 || row>=8 || col<0 || col>=8) { return NULL; }

    return board->Board[row][col];
}

GridCell* getCellByIndex(Board* board, int row, int col)
{
    if (row<0 || row>=8 || col<0 || col>=8) { return NULL; }
    return board->Board[row][col];
}

GridCell* getCellContainingPiece(Board* board, ChessPiece piece)
{
    for (size_t row = 0; row < 8; row++)
    {
        for (size_t col = 0; col < 8; col++)
        {
            GridCell *gc = board->Board[row][col];
            if (gc != NULL && gc->piece != NULL && gc->piece->piece == piece)
            {
                return  gc;
            }
        }
    }
    return NULL;
}

GridCellContainer* getAllCellsContainingPiece(Board* board, ChessPiece pieceType)
{
    if(board == NULL) { return NULL; }
    size_t count = 0;

    // Count the amount of those pieces on the board (used for dymanic memory allocation)
    for(size_t row=0; row<8; row++)
    {
        for(size_t col=0; col<8; col++)
        {
            if(board->Board[row][col]->piece->piece == pieceType)
            {
                count += 1;
            }
        }
    }

    if (count == 0) { return NULL; }

    GridCellContainer* container = (GridCellContainer*)malloc(sizeof(GridCellContainer));
    container->size = count;
    container->cellsArray = (GridCell**)malloc(container->size*sizeof(GridCell*));
    int offset = 0;
    for (size_t row = 0; row < 8; row++)
    {
        for(size_t col=0; col<8; col++)
        {
            if(board->Board[row][col]->piece->piece == pieceType)
            {
                container->cellsArray[offset] = board->Board[row][col];
                offset += 1; 
            }
        }
    }

    return container;
}

size_t countNumberOfPieceTypeOnBoard(Board* board, ChessPiece pieceType)
{
    size_t count = 0;

    for(size_t row=0; row<8; row++)
    {
        for(size_t col=0; col<8; col++)
        {
           if(board->Board[row][col]->piece->piece == pieceType) { count += 1; }
        }
    }

    return count;
}

Piece* getPiece(ChessPiece piece)
{
    Piece* newPiece = (Piece*)malloc(sizeof(Piece));
    switch (piece)
    {
    // WHITE PIECES
    case WHITE_CASTLE:
        newPiece->piece = WHITE_CASTLE;
        break;
    case WHITE_KNIGHT:
        newPiece->piece = WHITE_KNIGHT;
        break;
    case WHITE_BISHOP:
        newPiece->piece = WHITE_BISHOP;
        break;
    case WHITE_PAWN:
        newPiece->piece = WHITE_PAWN;
        break;
    case WHITE_KING:
        newPiece->piece = WHITE_KING;
        break;
    case WHITE_QUEEN:
        newPiece->piece = WHITE_QUEEN;
        break;

    // BLACK PIECES
    case BLACK_CASTLE:
        newPiece->piece = BLACK_CASTLE;
        break;
    case BLACK_KNIGHT:
        newPiece->piece = BLACK_KNIGHT;
        break;
    case BLACK_BISHOP:
        newPiece->piece = BLACK_BISHOP;
        break;
    case BLACK_PAWN:
        newPiece->piece = BLACK_PAWN;
        break;
    case BLACK_KING:
        newPiece->piece = BLACK_KING;
        break;
    case BLACK_QUEEN:
        newPiece->piece = BLACK_QUEEN;
        break;

    // EMPTY CELL
    case EMPTY:
        newPiece->piece = EMPTY;
        break;
    default:
        return NULL;
        break;
    }

    newPiece->id = getID();
    newPiece->moves = 0;
    newPiece->textureRect = getTextureRect(newPiece->piece);
    return newPiece;
}

node getValidCells(Board* board, GridCell* currentCell)
{
    if(currentCell != NULL && currentCell->piece != NULL)
    {
        ChessPiece pieceType = currentCell->piece->piece;
        node head = createNode();
        int row;
        int col;
        Pair positions[8];
        switch (pieceType)
        {
        case WHITE_PAWN:
        // Scope needed to prevent redefinition error involving GridCell* cell
            {
                // forward cell
                GridCell* cell = getCellByIndex(board, currentCell->row - 1, currentCell->col);
                if (cell != NULL)
                {
                    GridCell *originalCell = getCellByIndex(board, currentCell->row, currentCell->col);
                    if (!testCheck(board, originalCell, cell, PLAYER_WHITE))
                    {
                        head = addNode(head, cell);
                    }
                }
                // forward 2 cell
                if (!contains(board->pawnSet, currentCell->piece))
                {
                    cell = getCellByIndex(board, currentCell->row - 2, currentCell->col);
                    if (cell != NULL)
                    {
                        GridCell *originalCell = getCellByIndex(board, currentCell->row, currentCell->col);
                        if (!testCheck(board, originalCell, cell, PLAYER_WHITE))
                        {
                            head = addNode(head, cell);
                        }
                    }
                }
                insert(board->pawnSet, currentCell->piece);
            }
            return head->next;
            break;

        case BLACK_PAWN:
            // forward cell
            {
                GridCell* cell = getCellByIndex(board, currentCell->row + 1, currentCell->col);
                if (cell != NULL)
                {
                    GridCell *originalCell = getCellByIndex(board, currentCell->row, currentCell->col);
                    if (!testCheck(board, originalCell, cell, PLAYER_BLACK))
                    {
                        head = addNode(head, cell);
                    }
                }
                // forward 2 cell
                if (!contains(board->pawnSet, currentCell->piece))
                {
                    cell = getCellByIndex(board, currentCell->row + 2, currentCell->col);
                    if (cell != NULL)
                    {
                        GridCell *originalCell = getCellByIndex(board, currentCell->row, currentCell->col);
                        if (!testCheck(board, originalCell, cell, PLAYER_BLACK))
                        {
                            head = addNode(head, cell);
                        }
                    }
                }
                insert(board->pawnSet, currentCell->piece);
            }
            return head->next;
            break;

            case WHITE_CASTLE:
            case BLACK_CASTLE:
                row = currentCell->row;
                col = currentCell->col;
                Pair castleValidDirections[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
                for (size_t i = 0; i < 4; i++)
                {
                    while (row >= 0 && row < 8 && col >= 0 && col < 8)
                    {
                        row += castleValidDirections[i].x;
                        col += castleValidDirections[i].y;
                        GridCell *cell = getCellByIndex(board, row, col);
                        if (cell != NULL && cell->piece->piece == EMPTY)
                        {
                            GridCell *originalCell = getCellByIndex(board, currentCell->row, currentCell->col);
                            if (!testCheck(board, originalCell, cell, (pieceType == WHITE_CASTLE) ? PLAYER_WHITE : PLAYER_BLACK))
                            {
                                head = addNode(head, cell);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    row = currentCell->row;
                    col = currentCell->col;
                }
                return head->next;
                break;

            case WHITE_BISHOP:
            case BLACK_BISHOP:
                row = currentCell->row;
                col = currentCell->col;
                Pair bishopValidDirections[4] = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};

                for (size_t i = 0; i < 4; i++)
                {
                    while (row >= 0 && row < 8 && col >= 0 && col < 8)
                    {
                        row += bishopValidDirections[i].x;
                        col += bishopValidDirections[i].y;
                        GridCell *cell = getCellByIndex(board, row, col);
                        if (cell != NULL && cell->piece->piece == EMPTY)
                        {
                            GridCell *originalCell = getCellByIndex(board, currentCell->row, currentCell->col);
                            if (!testCheck(board, originalCell, cell, (pieceType == WHITE_BISHOP) ? PLAYER_WHITE : PLAYER_BLACK))
                            {
                                head = addNode(head, cell);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    row = currentCell->row;
                    col = currentCell->col;
                }

                return head->next;
                break;

            case WHITE_KNIGHT:
            case BLACK_KNIGHT:
                row = currentCell->row;
                col = currentCell->col;
                Pair knight_positions[8] = {{-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}};
                for (size_t i = 0; i < 8; i++)
                {
                    row = row + knight_positions[i].x;
                    col = col + knight_positions[i].y;

                    GridCell *cell = getCellByIndex(board, row, col);
                    if (cell != NULL && cell->piece->piece == EMPTY)
                    {
                        GridCell *originalCell = getCellByIndex(board, currentCell->row, currentCell->col);
                        if (!testCheck(board, originalCell, cell, (pieceType == WHITE_KNIGHT) ? PLAYER_WHITE : PLAYER_BLACK))
                        {
                            head = addNode(head, cell);
                        }
                    }

                    row = currentCell->row;
                    col = currentCell->col;
                }
                return head->next;
                break;

            case WHITE_QUEEN:
            case BLACK_QUEEN:
                row = currentCell->row;
                col = currentCell->col;
                Pair queenValidDirections[8] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
                for (size_t i = 0; i < 8; i++)
                {
                    while (row >= 0 && row < 8 && col >= 0 && col < 8)
                    {
                        row += queenValidDirections[i].x;
                        col += queenValidDirections[i].y;

                        GridCell *cell = getCellByIndex(board, row, col);
                        if (cell != NULL && cell->piece->piece == EMPTY)
                        {
                            GridCell *originalCell = getCellByIndex(board, currentCell->row, currentCell->col);
                            if (!testCheck(board, originalCell, cell, (pieceType == WHITE_QUEEN) ? PLAYER_WHITE : PLAYER_BLACK))
                            {
                                head = addNode(head, cell);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    row = currentCell->row;
                    col = currentCell->col;
                }
                return head->next;
                break;

            case WHITE_KING:
            case BLACK_KING:
                row = currentCell->row;
                col = currentCell->col;
                Pair king_positions[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};

                for (size_t i = 0; i < 8; i++)
                {
                    row = row + king_positions[i].x;
                    col = col + king_positions[i].y;

                    GridCell *cell = getCellByIndex(board, row, col);
                    if (cell != NULL && cell->piece->piece == EMPTY)
                    {
                        GridCell* originalCell = getCellByIndex(board, currentCell->row, currentCell->col); 
                        if(!testCheck(board, originalCell, cell, (pieceType == WHITE_KING) ? PLAYER_WHITE : PLAYER_BLACK))
                        {
                            head = addNode(head, cell);
                        }
                    }

                    row = currentCell->row;
                    col = currentCell->col;
                }
                return head->next;
                break;

            default:
                break;
            }
    }
    return NULL;
}

node getLineOfSightCells(Board* board, GridCell* gc)
{
    ChessPiece piece = gc->piece->piece;
    node head = createNode();
    head->gc = gc;
    int row;
    int col;
    switch (piece)
    {
        case WHITE_PAWN:
            row = gc->row;
            col = gc->col;
            // Pair whitePawnAttackPositions[4] = {{-1, -1}, {-1, 1}, {0, -1}, {0, 1}};
            Pair whitePawnAttackPositions[2] = {{-1, -1}, {-1, 1}};
            for(size_t i=0; i<2; i++) 
            {
                row += whitePawnAttackPositions[i].x;
                col += whitePawnAttackPositions[i].y;
                
                GridCell* cell = getCellByIndex(board, row, col);
                if(cell != NULL && cell->piece->piece != EMPTY)
                {
                    head = addNode(head, cell);
                }

                row = gc->row;
                col = gc->col;
            }
            return head;
            break;

        case BLACK_PAWN:
            row = gc->row;
            col = gc->col;
            Pair blackPawnAttackPositions[2] = {{1, -1}, {1, 1}};
            for(size_t i=0; i<2; i++) 
            {
                row += blackPawnAttackPositions[i].x;
                col += blackPawnAttackPositions[i].y;
                
                GridCell* cell = getCellByIndex(board, row, col);
                if(cell != NULL && cell->piece->piece != EMPTY)
                {
                    head = addNode(head, cell);
                }

                row = gc->row;
                col = gc->col;
            }
            return head;
            break;

        case WHITE_CASTLE:
        case BLACK_CASTLE:
            row = gc->row;
            col = gc->col;
            Pair castleValidDirections[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
            for(size_t i=0; i<4; i++)
            {
                while (row >= 0 && row < 8 && col >= 0 && col < 8)
                {
                    row += castleValidDirections[i].x;
                    col += castleValidDirections[i].y;

                    GridCell* cell = getCellByIndex(board, row, col);
                    if(cell != NULL)
                    {
                        head = addNode(head, cell);
                        if(cell->piece->piece != EMPTY) { break; }
                    }
                }
                row = gc->row;
                col = gc->col;
            }
            return head;
            break;

        case WHITE_BISHOP:
        case BLACK_BISHOP:
            row = gc->row;
            col = gc->col;

            Pair bishopValidDirections[4] = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
            for(size_t i=0; i<4; i++)
            {
                while (row >= 0 && row < 8 && col >= 0 && col < 8)
                {
                    row += bishopValidDirections[i].x;
                    col += bishopValidDirections[i].y;

                    GridCell* cell = getCellByIndex(board, row, col);
                    if(cell != NULL)
                    {
                        head = addNode(head, cell);
                        if(cell->piece->piece != EMPTY) { break; }
                    }
                }
                row = gc->row;
                col = gc->col;
            }
           
            return head;
            break;

        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            row = gc->row;
            col = gc->col;
            Pair knight_positions[8] = {{-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}};
            for (size_t i = 0; i < 8; i++)
            {
                row = row + knight_positions[i].x;
                col = col + knight_positions[i].y;

                GridCell* cell = getCellByIndex(board, row, col);
                if (cell != NULL)
                {
                    head = addNode(head, cell);
                }

                row = gc->row;
                col = gc->col;
            }
            return head;
            break;

        case WHITE_QUEEN:
        case BLACK_QUEEN:
            row = gc->row;
            col = gc->col;

            Pair queenValidDirections[8] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
            for(size_t i=0; i<8; i++)
            {
                while (row >= 0 && row < 8 && col >= 0 && col < 8)
                {
                    row += queenValidDirections[i].x;
                    col += queenValidDirections[i].y;

                    GridCell* cell = getCellByIndex(board, row, col);
                    if(cell != NULL)
                    {
                        head = addNode(head, cell);
                        if(cell->piece->piece != EMPTY) { break; }
                    }
                }
                row = gc->row;
                col = gc->col;
            }

            return head;
            break;

        case WHITE_KING:
        case BLACK_KING:
            row = gc->row;
            col = gc->col;
            Pair king_positions[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};

            for (size_t i = 0; i < 8; i++)
            {
                row = row + king_positions[i].x;
                col = col + king_positions[i].y;

                GridCell* cell = getCellByIndex(board, row, col);
                if (cell != NULL)
                {
                    head = addNode(head, cell);
                }

                row = gc->row;
                col = gc->col;
            }
            return head;
            break;

        default:
            break;

    }
    return NULL;

}

// TODO: Should only return capture cells which don't lead to check conditions 
node getCaptureCells(Board* board, GridCell* gc)
{
    if(gc != NULL && gc->piece != NULL && gc->piece->piece != EMPTY)
    {
        node head = NULL;
        node cur = getLineOfSightCells(board, gc);
        while (cur != NULL)
        {
            if (isWhitePiece(gc->piece))
            {
                if (isBlackPiece(cur->gc->piece))
                {
                    
                    if (head == NULL)
                    {
                        head = createNode();
                        head->gc = cur->gc;
                    }
                    else
                    {
                        head = addNode(head, cur->gc);
                    }
                    
                }
            }

            else if (isBlackPiece(gc->piece))
            {
                if (isWhitePiece(cur->gc->piece))
                {
                    if (head == NULL)
                    {
                        head = createNode();
                        head->gc = cur->gc;
                    }
                    else
                    {
                        head = addNode(head, cur->gc);
                    }
                }
            }

            cur = cur->next;
        }

        if ((gc->piece->piece == WHITE_PAWN) || (gc->piece->piece == BLACK_PAWN))
        {
            node enPassantHead = getEnPassantCells(board, gc);
            if (enPassantHead != NULL) { head = concatenate_LL(head, enPassantHead); }
        }

        return head;
    }
}

node getEnPassantCells(Board* board, GridCell* gc)
{
    // TODO: Add error handling here
    if(board == NULL || gc == NULL) { return NULL; }

    node head = NULL;
    if(gc->piece != NULL)
    {
        
        if(isWhitePiece(gc->piece))
        {
            // check left
            {
                GridCell *neighbour = getCellByIndex(board, gc->row, gc->col - 1);
                if ((neighbour != NULL) && (neighbour->piece->piece == BLACK_PAWN) && (neighbour->piece->moves == 1))
                {
                    // Add En Passant Cell
                    GridCell *enPassantCell = getCellByIndex(board, (gc->row - 1), (gc->col - 1));
                    if (enPassantCell != NULL)
                    {
                        head = addNode(head, enPassantCell);
                    }
                }
            }

            // check right
            {
                GridCell *neighbour = getCellByIndex(board, gc->row, gc->col + 1);
                if ((neighbour != NULL) && (neighbour->piece->piece == BLACK_PAWN) && (neighbour->piece->moves == 1))
                {
                    // Add En Passant Cell
                    GridCell *enPassantCell = getCellByIndex(board, (gc->row - 1), (gc->col + 1));
                    if (enPassantCell != NULL)
                    {
                        head = addNode(head, enPassantCell);
                    }
                }
            }
        }
        else if (isBlackPiece(gc->piece))
        {
            // check left
            {
                GridCell *neighbour = getCellByIndex(board, gc->row, gc->col - 1);
                if ((neighbour != NULL) && (neighbour->piece->piece == WHITE_PAWN) && (neighbour->piece->moves == 1))
                {
                    // Add En Passant Cell
                    GridCell *enPassantCell = getCellByIndex(board, (gc->row + 1), (gc->col - 1));
                    if (enPassantCell != NULL)
                    {
                        head = addNode(head, enPassantCell);
                    }
                }
            }

            // check right
            {
                GridCell *neighbour = getCellByIndex(board, gc->row, gc->col + 1);
                if ((neighbour != NULL) && (neighbour->piece->piece == WHITE_PAWN) && (neighbour->piece->moves == 1))
                {
                    // Add En Passant Cell
                    GridCell *enPassantCell = getCellByIndex(board, (gc->row + 1), (gc->col + 1));
                    if (enPassantCell != NULL)
                    {
                        head = addNode(head, enPassantCell);
                    }
                }
            }
        }
    }

    return head;
}

node getCastlingCells(Board* board, GridCell* gc)
{
    if(board == NULL || gc == NULL) { return NULL; }
    node head = NULL;
    if(isAllowedToCastle(board, gc))
    {
        ChessPiece pieceType = gc->piece->piece;
        if(isAllowedToCastleQueenSide(board, gc)) 
        {
            if(pieceType == WHITE_KING)
            {
                head = addNode(head, getCellByIndex(board, 7, 2));
            }
            else if (pieceType == BLACK_KING)
            {
                head = addNode(head, getCellByIndex(board, 0, 2));
            }
        }

        if (isAllowedToCastleKingSide(board, gc))
        {
            if(pieceType == WHITE_KING)
            {
                head = addNode(head, getCellByIndex(board, 7, 6));
            }
            else if (pieceType == BLACK_KING)
            {
                head = addNode(head, getCellByIndex(board, 0, 6));
            }
        }
    }
    return head;
}


void performCastle(Board* board, GridCell* kingDropCell)
{
    Piece* kingPiece = kingDropCell->piece;
    if(kingPiece == NULL) { return; }

    if (kingDropCell->col == 6)
    {
        performKingSideCastle(board, kingDropCell);
    }
    else if(kingDropCell->col == 2)
    {
        performQueenSideCastle(board, kingDropCell);
    }
    
}

void performKingSideCastle(Board* board, GridCell* kingDropCell)
{
    Piece* kingPiece = kingDropCell->piece;
    if(kingPiece->piece == WHITE_KING)
    {
        // Right side castle for white
        GridCell* castleGridCell = getCellByIndex(board, 7, 7);    
        Piece* castle = castleGridCell->piece;
        // "Empty" current castle cell
        updateBoard(board, castleGridCell->row, castleGridCell->col, NULL);
        // Add castle to new cell
        updateBoard(board, 7, 5, castle);
    }

    else if(kingPiece->piece == BLACK_KING)
    {
       // Right side castle for black
        GridCell* castleGridCell = getCellByIndex(board, 0, 7);    
        Piece* castle = castleGridCell->piece;
        updateBoard(board, castleGridCell->row, castleGridCell->col, NULL);
        updateBoard(board, 0, 5, castle);
    }
}

void performQueenSideCastle(Board* board, GridCell* kingDropCell)
{
    Piece* kingPiece = kingDropCell->piece;
    if(kingPiece->piece == WHITE_KING)
    {
        // Left side castle for white
        GridCell* castleGridCell = getCellByIndex(board, 7, 0);    
        Piece* castle = castleGridCell->piece;
        updateBoard(board, castleGridCell->row, castleGridCell->col, NULL);
        updateBoard(board, 7, 3, castle);
    }

    else if(kingPiece->piece == BLACK_KING)
    {
       // Left side castle for black
        GridCell* castleGridCell = getCellByIndex(board, 0, 0);    
        Piece* castle = castleGridCell->piece;
        updateBoard(board, castleGridCell->row, castleGridCell->col, NULL);
        updateBoard(board, 0, 3, castle);
    }
}

Bool isWhitePiece(Piece* p)
{
    return (p->piece >= WHITE_PAWN && p->piece <= WHITE_KING);
} 

Bool isBlackPiece(Piece* p)
{
    return (p->piece >= BLACK_PAWN && p->piece <= BLACK_KING);
}

Bool isAllowedToCastle(Board* board, GridCell* gc)
{
    if (board == NULL || gc == NULL)
    {
        // TODO: handle error properly
        printf("Error board.c isAllowedToCastle\n");
    }

    Piece* piece = gc->piece;
    
    if(piece->piece == WHITE_KING || piece->piece == BLACK_KING)
    {
        // if the king has already moved before or there are no castle's on the board
        if (piece->moves >= 1 || countNumberOfPieceTypeOnBoard(board, (piece->piece == WHITE_KING) ? WHITE_CASTLE : BLACK_CASTLE) < 1) { return False; }

        // locate the castle cells
        // GridCell* castleCells[countNumberOfPieceTypeOnBoard(board, (piece->piece == WHITE_KING) ? WHITE_CASTLE : BLACK_CASTLE)];
        // size_t castleCellsSize = 0;

        // When checking if cells between the castle and the king are empty. We can assume that we need to check the horizontal cells between
        // the kings original position and the castles original position only (since it must be both the kings and the castles first move)
        // e.g for the white king and the queenside castle we would check cells (e1, f1, g1)
        // *note Pam Krabbe castling (vertical castling between a king and promoted castle) is not allowed in the FIDE rule set.
        // (https://de-m-wikipedia-org.translate.goog/wiki/Pam-Krabb%C3%A9-Rochade?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en)

        return (isAllowedToCastleQueenSide(board, gc) || isAllowedToCastleKingSide(board, gc));
    }

    return False;
}

Bool isAllowedToCastleQueenSide(Board* board, GridCell* gc)
{
    // TODO: Add error handling instead of just returning
    if(board == NULL || gc == NULL) { return False; }

    Piece* piece = gc->piece;
    int row; 
    int col;
    Pair castlingCheckDirectionsLeft = {0, -1};
    Bool emptySpaceBetweenKingAndCastle = False;
    Bool kingCanMoveWithoutGoingThroughCheck = True;
    size_t numCellsToCheckQueenSide = 4;

    // check spaces between king and castle are empty
    row = gc->row;
    col = gc->col;
    row += castlingCheckDirectionsLeft.x;
    col += castlingCheckDirectionsLeft.y;

    while (row >= 0 && row < 8 && col >= 0 && col < 8)
    {
        GridCell *cell = getCellByIndex(board, row, col);
        if (cell->piece->piece == ((piece->piece == WHITE_KING) ? WHITE_CASTLE : BLACK_CASTLE))
        {
            emptySpaceBetweenKingAndCastle |= True;
            // castleCells[castleCellsSize++] = cell;
            break;
        }
        else if (cell->piece->piece != EMPTY)
        {
            emptySpaceBetweenKingAndCastle |= False;
            break;
        }
        row += castlingCheckDirectionsLeft.x;
        col += castlingCheckDirectionsLeft.y;
    }

    if(!emptySpaceBetweenKingAndCastle) { return False; }

    // check king will not pass through check when castling
    row = gc->row;
    col = gc->col;

    Board *testBoard = deepCopyBoard(board);

    GridCell *testKingCell = getCellByIndex(testBoard, row, col);
    Piece *testKing = deepCopyPiece(testKingCell->piece);

    for(size_t i=0; i<numCellsToCheckQueenSide; i++)
    {
        // Bounds check
        if (row < 0 || row >= 8 || col < 0 || col >= 8 || (board->Board[row][col]->piece->piece == ((piece->piece == WHITE_KING) ? WHITE_CASTLE : BLACK_CASTLE)))
        {
            break;
        }
        updateBoard(testBoard, row, col, testKing);
        if (isInCheck(testBoard, ((piece->piece == WHITE_KING) ? PLAYER_WHITE : PLAYER_BLACK)))
        {
            kingCanMoveWithoutGoingThroughCheck &= False;
            break;
        }

        updateBoard(testBoard, row, col, NULL);
        row += castlingCheckDirectionsLeft.x;
        col += castlingCheckDirectionsLeft.y;
    }

    return (kingCanMoveWithoutGoingThroughCheck && emptySpaceBetweenKingAndCastle);
}

Bool isAllowedToCastleKingSide(Board* board, GridCell* gc)
{
    // TODO: Add error handling instead of just returning
    if(board == NULL || gc == NULL) { return False; }

    Piece* piece = gc->piece;
    int row; 
    int col;
    Pair castlingCheckDirectionsRight = {0, 1};
    Bool emptySpaceBetweenKingAndCastle = False;
    Bool kingCanMoveWithoutGoingThroughCheck = True;
    size_t numCellsToCheckKingSide = 3;

    // Check for empty spaces between king and castle
    row = gc->row;
    col = gc->col;
    row += castlingCheckDirectionsRight.x;
    col += castlingCheckDirectionsRight.y;

    while (row >= 0 && row < 8 && col >= 0 && col < 8)
    {
         
        GridCell *cell = getCellByIndex(board, row, col);
        if (cell->piece->piece == ((piece->piece == WHITE_KING) ? WHITE_CASTLE : BLACK_CASTLE))
        {
            emptySpaceBetweenKingAndCastle |= True;
            // castleCells[castleCellsSize++] = cell;
            break;
        }
        else if (cell->piece->piece != EMPTY)
        {
            emptySpaceBetweenKingAndCastle |= False;
            break;
        }
        row += castlingCheckDirectionsRight.x;
        col += castlingCheckDirectionsRight.y;
    }

    if(!emptySpaceBetweenKingAndCastle) { return False; }

    // Check king will not pass thorough check when castling
    row = gc->row;
    col = gc->col;
    Board* testBoard = deepCopyBoard(board);
    GridCell* testKingCell = getCellByIndex(testBoard, row, col);
    Piece* testKing = deepCopyPiece(testKingCell->piece);

    for(size_t i=0; i<numCellsToCheckKingSide; i++)
    { 
        // Bounds check
        if (row < 0 || row >= 8 || col < 0 || col >= 8 || (board->Board[row][col]->piece->piece == ((piece->piece == WHITE_KING) ? WHITE_CASTLE : BLACK_CASTLE)))
        {
            break;
        }
        updateBoard(testBoard, row, col, testKing);
        if (isInCheck(testBoard, (piece->piece == WHITE_KING) ? PLAYER_WHITE : PLAYER_BLACK))
        {
            kingCanMoveWithoutGoingThroughCheck &= False;
            break;
        }

        updateBoard(testBoard, row, col, NULL);
        row += castlingCheckDirectionsRight.x;
        col += castlingCheckDirectionsRight.y;
    }

    return (kingCanMoveWithoutGoingThroughCheck && emptySpaceBetweenKingAndCastle);
}

node createNode()
{
    node temp;
    temp = (node)malloc(sizeof(struct LinkedList));
    temp->next = NULL;
    return temp;
}

node addNode(node head, GridCell* value)
{
    node temp, p;
    temp = createNode();
    temp->gc = value;
    if(head == NULL)
    {
        head = temp;
    }
    else
    {
        p = head;
        while(p->next != NULL)
        {
            p = p->next;
        }
        p->next = temp;
    }

    return head;
}

node concatenate_LL(node head1, node head2)
{
    if(head1 == NULL) { return head2; }
    if(head2 == NULL) { return head1; }

    // find tail of list 1
    node cur = head1;
    while(cur->next != NULL)
    {
        cur = cur->next;
    }

    // contatenate lists
    cur->next = head2;

    return head1;
}

void freeList(node head)
{
    node cur = head;
    node next = NULL;

    while(cur != NULL)
    {
        next = cur->next;
        cur->gc = NULL;
        cur->next = NULL;
        free(cur);
        cur = next;
    }
}

Bool contains_LL(node head, GridCell* gc)
{
    node cur = head;
    while(cur != NULL)
    {
        if(cur->gc == gc) { return True; }
        cur = cur->next;
    }
    return False;
}

node remove_LL(node head, GridCell* gc)
{
    if(head == NULL || gc == NULL) { return NULL; }

    node dummy = createNode();
    dummy->next = head;
    node back = dummy;
    node front = head;
    while(front != NULL)
    {
        if(front->gc != NULL && front->gc->row == gc->row && front->gc->col == gc->col)
        {
            node n = front->next;

            front->next = NULL;
            front->gc = NULL;

            if(n != NULL)
            {
                back->next = n;
            }

            free(front);
            return dummy->next;
        }

        back = front;
        front = front->next;
    }

    return dummy->next;
}

Bool isValidGridCell(GridCell* gc, node head)
{
    node cur = head;
    while(cur != NULL)
    {
        if(gc->row == cur->gc->row && gc->col == cur->gc->col)
        {
            return True;
        }
        cur = cur->next;
    }
    return False;
}

Rectangle getTextureRect(ChessPiece piece)
{
    Rectangle textureClipRect = {.x=0, .y=0, .width=PIECE_WIDTH, .height=PIECE_HEIGHT};
    switch (piece)
    {
    // WHITE PIECES
    case WHITE_CASTLE:
        textureClipRect.x = 0 * PIECE_WIDTH;
        textureClipRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_KNIGHT:
        textureClipRect.x = 1 * PIECE_WIDTH;
        textureClipRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_BISHOP:
        textureClipRect.x = 2 * PIECE_WIDTH;
        textureClipRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_PAWN:
        textureClipRect.x = 3 * PIECE_WIDTH;
        textureClipRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_KING:
        textureClipRect.x = 4 * PIECE_WIDTH;
        textureClipRect.y = 0 * PIECE_HEIGHT;
        break;
    case WHITE_QUEEN:
        textureClipRect.x = 5 * PIECE_WIDTH;
        textureClipRect.y = 0 * PIECE_HEIGHT;
        break;

    // BLACK PIECES
    case BLACK_CASTLE:
        textureClipRect.x = 0 * PIECE_WIDTH;
        textureClipRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_KNIGHT:
        textureClipRect.x = 1 * PIECE_WIDTH;
        textureClipRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_BISHOP:
        textureClipRect.x = 2 * PIECE_WIDTH;
        textureClipRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_PAWN:
        textureClipRect.x = 3 * PIECE_WIDTH;
        textureClipRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_KING:
        textureClipRect.x = 4 * PIECE_WIDTH;
        textureClipRect.y = 1 * PIECE_HEIGHT;
        break;
    case BLACK_QUEEN:
        textureClipRect.x = 5 * PIECE_WIDTH;
        textureClipRect.y = 1 * PIECE_HEIGHT;
        break;

    default:
        textureClipRect.x = 0;
        textureClipRect.y = 0;
        textureClipRect.width = 0;
        textureClipRect.height = 0;
        break;
    }

    return textureClipRect;
}

Bool isInCheck(Board* board, Player player)
{
    GridCell* kingGridCell;
    if(player == PLAYER_WHITE)
    {
        kingGridCell = getCellContainingPiece(board, WHITE_KING);
        if(kingGridCell != NULL)
        {
            for(size_t row=0; row<8; row++)       
            {
                for(size_t col=0; col<8; col++)
                {
                    GridCell* gc = getCellByIndex(board, row, col);
                    if(gc != NULL && gc->piece != NULL && isBlackPiece(gc->piece))
                    {
                        node enemyCaptureCells = getCaptureCells(board, gc);
                        if (contains_LL(enemyCaptureCells, kingGridCell))
                        {
                            return True;
                        }
                    }
                }
            }
        }
    }
    else if (player == PLAYER_BLACK)
    {
        kingGridCell = getCellContainingPiece(board, BLACK_KING);
        if (kingGridCell != NULL)
        {
            for (size_t row = 0; row < 8; row++)
            {
                for (size_t col = 0; col < 8; col++)
                {
                    GridCell *gc = getCellByIndex(board, row, col);
                    if (gc != NULL && gc->piece != NULL && isWhitePiece(gc->piece))
                    {
                        node enemyCaptureCells = getCaptureCells(board, gc);
                        if(contains_LL(enemyCaptureCells, kingGridCell))
                        {
                            return True;
                        }
                    }
                }
            }
        }
    }
    return False;
}

Bool testCheck(Board* board, GridCell* originCell, GridCell* destinationCell, Player player)
{
    if(board == NULL || originCell == NULL || originCell->piece == NULL || destinationCell == NULL) 
    { 
        // TODO: Handle error properly
        printf("ERROR: board.c testCheck\n");
        return False;
    }

    Board *testBoard = deepCopyBoard(board);
    Piece *testPiece = deepCopyPiece(originCell->piece);

    // Remove old piece
    updateBoard(testBoard, originCell->row, originCell->col, NULL);
    // Add piece to test positon
    updateBoard(testBoard, destinationCell->row, destinationCell->col, testPiece);

    Bool res = isInCheck(testBoard, player);

    freeBoard(testBoard);
    return res;
}

Board* deepCopyBoard(Board* oringinalBoard)
{
    if(oringinalBoard == NULL) { return NULL; }
    Board* newBoard = (Board*)malloc(sizeof(Board));

    // copy colour board
    for (size_t row = 0; row < 8; row++)
    {
        for(size_t col=0; col<8; col++)
        {
            newBoard->colourBoard[row][col] = oringinalBoard->colourBoard[row][col];
        }
    }

    // deep copy board
    for(size_t row=0; row<8; row++)
    {
        for(size_t col=0; col<8; col++)
        {
            GridCell* newGC = deepCopyGridCell(oringinalBoard->Board[row][col]); //<-- BUG(1) newGC is NULL
            if(newGC == NULL) { return NULL; }
            newBoard->Board[row][col] = newGC;
        }
    }

    newBoard->mainTexture = oringinalBoard->mainTexture;
    newBoard->pawnSet = NULL; // test boards don't require a pawnSet ATM;

    return newBoard;
}

void freeBoard(Board* board)
{
    if(board == NULL) { return; }

    // Free grid cell pointers
    for(size_t row=0; row<8; row++)
    {
        for(size_t col=0; col<8; col++)
        {
            freeGridCell(board->Board[row][col]);
            board->Board[row][col] = NULL;
        }
    }

    // Free pawn set (hash set)
    freeHashSet(board->pawnSet);

    free(board);
}

HashNode* createHashNode(Piece* piece)
{
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    if(newNode == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    newNode->piece = piece;
    newNode->next = NULL;
    return newNode;
}

HashSet* createHashSet(){
    HashSet* set = (HashSet*)malloc(sizeof(HashSet));
    if(set == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    for(size_t i=0; i<SIZE; i++)
    {
        set->buckets[i] = NULL;
    }

    return set;
}

void freeHashSet(HashSet* set)
{
    if (set == NULL) { return; }
    for(size_t i=0; i<SIZE; i++)
    {
        HashNode *cur = set->buckets[i];
        HashNode *next = NULL;
        while (cur != NULL)
        {
            // Free piece struct contained in bucket
            if (cur->piece != NULL)
            {
                free(cur->piece);
                cur->piece = NULL;
            }

            // Free node pointer
            next = cur->next;
            free(cur);
            cur = next;
        }
        set->buckets[i] = NULL;
    }

    free(set);
}

int hash(int key)
{
    return key % SIZE;
}

void insert(HashSet* set, Piece* piece)
{
    if(!contains(set, piece))
    {
        int index = hash(piece->id);
        HashNode *newNode = createHashNode(piece);
        newNode->next = set->buckets[index];
        set->buckets[index] = newNode;
    }
}

bool contains(HashSet* set, Piece* piece)
{
    int index = hash(piece->id);
    HashNode* current = set->buckets[index];
    while (current != NULL)
    {
        if(current->piece->id == piece->id)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

// ###################################################### DEBUG FUNCTIONS ###################################################################
void printBoard(Board* board, char* title)
{
    if(board != NULL)
    {
        if(title != NULL)
        {
            printf("%s\n", title);
        }
        printf("********************************************************\n");
        printf("[");
        for(size_t i=0; i<8; i++)
        {
            for(size_t j=0; j<8; j++)
            {
                printf(" %d, ", board->Board[i][j]->piece->piece);
            }
            printf("]\n");
        }
        printf("********************************************************\n");
    }
}

void printLL(node head)
{
    node cur = head;
    while(cur != NULL)
    {
        printf("Row: %d, Col: %d\n", cur->gc->row, cur->gc->col);
        cur = cur->next;
    }
}
// ###################################################### END DEBUG FUNCTIONS ################################################################