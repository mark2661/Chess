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

    default:
        return NULL;
        break;
    }

    newPiece->id = getID();
    newPiece->textureRect = getTextureRect(newPiece->piece);
    return newPiece;
}

node getValidCells(Board* board, GridCell* currentCell)
{
    if(currentCell != NULL && currentCell->piece != NULL)
    {
        ChessPiece pieceType = currentCell->piece->piece;
        node head = createNode();
        // TODO: don't include currentCell in validCells list
        head->gc = currentCell;
        int row;
        int col;
        Pair positions[8];
        switch (pieceType)
        {
        case WHITE_PAWN:
        // Scope needed to prevent redefinition error involving GridCell* cell
            {
                // forward cell
                GridCell* cell = getCellByIndex(board, currentCell->row + 1, currentCell->col);
                if (cell != NULL)
                {
                    addNode(head, cell);
                }
                // forward 2 cell
                if (!contains(board->pawnSet, currentCell->piece))
                {
                    cell = getCellByIndex(board, currentCell->row + 2, currentCell->col);
                    if (cell != NULL)
                    {
                        addNode(head, cell);
                    }
                }
                insert(board->pawnSet, currentCell->piece);
            }
            return head;
            break;

        case BLACK_PAWN:
            // forward cell
            {
                GridCell* cell = getCellByIndex(board, currentCell->row - 1, currentCell->col);
                if (cell != NULL)
                {
                    addNode(head, cell);
                }
                // forward 2 cell
                if (!contains(board->pawnSet, currentCell->piece))
                {
                    cell = getCellByIndex(board, currentCell->row - 2, currentCell->col);
                    if (cell != NULL)
                    {
                        addNode(head, cell);
                    }
                }
                insert(board->pawnSet, currentCell->piece);
            }
            return head;
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
                            addNode(head, cell);
                        }
                        else
                        {
                            break;
                        }
                    }
                    row = currentCell->row;
                    col = currentCell->col;
                }
                return head;
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
                            addNode(head, cell);
                        }
                        else
                        {
                            break;
                        }
                    }
                    row = currentCell->row;
                    col = currentCell->col;
                }

                return head;
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
                        addNode(head, cell);
                    }

                    row = currentCell->row;
                    col = currentCell->col;
                }
                return head;
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
                            addNode(head, cell);
                        }
                        else
                        {
                            break;
                        }
                    }
                    row = currentCell->row;
                    col = currentCell->col;
                }
                return head;
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
                        addNode(head, cell);
                    }

                    row = currentCell->row;
                    col = currentCell->col;
                }
                return head;
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
            Pair whitePawnAttackPositions[2] = {{1, -1}, {1, 1}};
            for(size_t i=0; i<2; i++) 
            {
                row += whitePawnAttackPositions[i].x;
                col += whitePawnAttackPositions[i].y;
                
                GridCell* cell = getCellByIndex(board, row, col);
                if(cell != NULL && cell->piece->piece != EMPTY)
                {
                    addNode(head, cell);
                }

                row = gc->row;
                col = gc->col;
            }
            return head;
            break;

        case BLACK_PAWN:
            row = gc->row;
            col = gc->col;
            Pair blackPawnAttackPositions[2] = {{-1, -1}, {-1, 1}};
            for(size_t i=0; i<2; i++) 
            {
                row += blackPawnAttackPositions[i].x;
                col += blackPawnAttackPositions[i].y;
                
                GridCell* cell = getCellByIndex(board, row, col);
                if(cell != NULL && cell->piece->piece != EMPTY)
                {
                    addNode(head, cell);
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
                        addNode(head, cell);
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
                        addNode(head, cell);
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
                    addNode(head, cell);
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
                        addNode(head, cell);
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
                    addNode(head, cell);
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
                        addNode(head, cur->gc);
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
                        addNode(head, cur->gc);
                    }
                }
            }

            cur = cur->next;
        }

        return head;
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