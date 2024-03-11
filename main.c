#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "board.h"
#include "menu.h"
#include "raylib.h"

#define PIECE_SELECT_MENU_OPTIONS {"CASTLE", "KNIGHT", "BISHOP", "QUEEN"}
#define PIECE_SELECT_MENU_OPTIONS_LENGTH 4
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
GameState state = WHITE_IN_PLAY;

Menu* menu = NULL;
GridCell* pawnPromotionCell = NULL;

char *options[PIECE_SELECT_MENU_OPTIONS_LENGTH] = PIECE_SELECT_MENU_OPTIONS;
size_t options_length = PIECE_SELECT_MENU_OPTIONS_LENGTH;

// DEBUG
int count = 0;
// GameState state = WHITE_PIECE_SELECT_MENU;
// DEBUG

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
        case WHITE_PIECE_SELECT_MENU:
            pieceSelectMenuIteration(&board, PLAYER_WHITE);
            break;
        case BLACK_PIECE_SELECT_MENU:
            pieceSelectMenuIteration(&board, PLAYER_BLACK);
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
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !dragging)
    {
        dragPiece = startDragOperation(board, player);
    }

    else if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && dragging && dragPiece != NULL)
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
    // Render menu title
    if(menu->title != NULL)
    {
        menu->title->messagePosition.y = MENU_TITLE_START_HEIGHT;
        DrawText(menu->title->message, menu->title->messagePosition.x, menu->title->messagePosition.y, MENU_TEXT_FONT_SIZE, BLACK);
    }

    // Render menu items
    for(size_t i=0; i<PIECE_SELECT_MENU_OPTIONS_LENGTH; i++)
    {
        MenuItem *item = menu->menuItems[i];
        if (item != NULL)
        {
            DrawText(item->message, item->messagePosition.x, item->messagePosition.y, MENU_TEXT_FONT_SIZE, BLACK);

            // Draw bounding box if mousing over text
            if (CheckCollisionPointRec(GetMousePosition(), item->boundingBox))
            {
                DrawRectangleLinesEx(item->boundingBox, 3.0f, RED);
            }
            // detect click
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), item->boundingBox))
            {
                pawnPromotionCell->piece->piece = item->piece;
                pawnPromotionCell->piece->textureRect = getTextureRect(item->piece);

                if(player == PLAYER_WHITE) { state = BLACK_IN_PLAY; }
                else if (player == PLAYER_BLACK) { state = WHITE_IN_PLAY; }
            }
        }
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

        // When I check I should Allow any move which gets the player out of check (including blocking and captures)

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
            if(board == NULL || dragPiece == NULL) { return; }

            dragging = False;
            Bool moveAccepted = False;
            Piece* piece = getNewPiece(dragPiece);
            Piece* testPiece = deepCopyPiece(piece); // DO NOT REMOVE the testBoard must only be updated using the testPiece variable (a copy of the piece variable)
            Board* testBoard = deepCopyBoard(board);

            GridCell* gc = getCellByMousePosition(board);
            if(gc != NULL && gc->piece != NULL && testBoard != NULL)
            {
                // Move to empty cell
                if (gc->piece->piece == EMPTY && isValidGridCell(gc, dragPiece->validCells))
                {
                    // TODO: edit valid and capture cells of piece to only show moves which would resolve the check condtition
                    updateBoard(testBoard, gc->row, gc->col, testPiece);
                    if(!isInCheck(testBoard, (state == WHITE_IN_PLAY) ? PLAYER_WHITE : PLAYER_BLACK))
                    {
                        // Accept move
                        updateBoard(board, gc->row, gc->col, piece);
                        state = (state == WHITE_IN_PLAY) ? BLACK_IN_PLAY : WHITE_IN_PLAY;

                        moveAccepted = True;
                    }
                    
                }
                // Capture piece and move to cell
                else if (gc->piece->piece != EMPTY && isValidGridCell(gc, dragPiece->captureCells))
                {
                    updateBoard(testBoard, gc->row, gc->col, testPiece);
                    if(!isInCheck(testBoard, (state == WHITE_IN_PLAY) ? PLAYER_WHITE : PLAYER_BLACK))
                    {
                        // remove captured piece
                        freePiece(gc);
                        updateBoard(board, gc->row, gc->col, piece);
                        state = (state == WHITE_IN_PLAY) ? BLACK_IN_PLAY : WHITE_IN_PLAY;
                        moveAccepted = True;
                    }
              }
               // Invalid move return to origin cell
               if (!moveAccepted)
               {

                   // TODO: Fix bug where a pawn's initial move is invalid but it is still added to the pawnSet meaning it
                   // no longer has the option of moving two squares in front
                   GridCell *originalGridCell = getCellByIndex(board, dragPiece->originalPosition.x, dragPiece->originalPosition.y);
                   if (originalGridCell != NULL)
                   {
                       updateBoard(board, originalGridCell->row, originalGridCell->col, piece);
                   }
                }

                // TODO: create a function which encapsulates all the logic associated with state transition
                // maybe don't need to do this anymore?
                if (gc->piece->piece == WHITE_PAWN && gc->row == 7)
                {
                    menu = createMenu("Pawn Promoted: Select New Piece", options, options_length, PLAYER_WHITE);
                    pawnPromotionCell = gc;
                    state = WHITE_PIECE_SELECT_MENU;
                }
                else if(gc->piece->piece == BLACK_PAWN && gc->row == 0)
                {
                    menu = createMenu("Pawn Promoted: Select New Piece", options, options_length, PLAYER_BLACK);
                    pawnPromotionCell = gc;
                    state = BLACK_PIECE_SELECT_MENU;
                }
            }

            if(testBoard != NULL) { freeBoard(testBoard); }
            if(dragPiece != NULL) { freeDragPiece(dragPiece); }
            resetColourBoard(board);
}