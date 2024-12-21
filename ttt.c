#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.c"

unsigned int BOARD_HEIGHT = 3;
unsigned int BOARD_WIDTH = 3;
unsigned int IN_A_ROW = 3;
#define BOARD_CELLS BOARD_WIDTH *BOARD_HEIGHT

#define ERROR_FAILED_TO_ALLOCATE "Failed to allocate %ld bytes of memory!"

typedef enum Player
{
    X,
    O,
    NONE
} Player;

typedef struct GameState
{
    Player turn;
    Player *board;
} GameState;

GameState *empty_state(Player firstTurn)
{
    GameState *state = malloc(sizeof(GameState));
    if (!state)
    {
        printf(ERROR_FAILED_TO_ALLOCATE, sizeof(GameState));
        exit(1);
    }
    state->turn = firstTurn;
    state->board = malloc(sizeof(Player) * BOARD_CELLS);
    if (!state->board)
    {
        printf(ERROR_FAILED_TO_ALLOCATE, sizeof(Player) * BOARD_CELLS);
        exit(1);
    }
    for (unsigned int i = 0; i < BOARD_CELLS; i++)
    {
        state->board[i] = NONE;
    }
    return state;
}

void print_board(GameState *state)
{
    unsigned int max_num_len = floor(log10(abs(BOARD_CELLS))) + 1;
    for (unsigned int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (unsigned int j = 0; j < BOARD_WIDTH; j++)
        {
            printf("[");
            switch (state->board[i * BOARD_HEIGHT + j])
            {
            case X:
                printf("X");
                break;
            case O:
                printf("O");
                break;
            case NONE:
                printf(" ");
                break;
            }
            printf("]");
        }
        printf("\t\t\t");
        for (unsigned int j = 0; j < BOARD_WIDTH; j++)
        {
            unsigned int this_num = i * BOARD_HEIGHT + j + 1;
            unsigned int this_num_len = floor(log10(abs(this_num))) + 1;
            printf("[");
            for (unsigned int i = 0; i < max_num_len - this_num_len; i++)
                printf(" ");
            printf("%d]", this_num);
        }
        printf("\n");
    }
}

void make_move_from_input(GameState *state)
{

    char buf[8] = {0};
    long num = 0;
    while (1)
    {
        if (state->turn == X)
            printf("X turn (%d-%d): ", 1, BOARD_CELLS);
        else
            printf("O turn (%d-%d): ", 1, BOARD_CELLS);
        fflush(stdin);
        fgets(buf, sizeof(buf), stdin);
        num = strtolong(buf);
        num -= 1;
        if (num < 0 || num >= BOARD_CELLS)
        {
            printf("Invalid Input\n");
            continue;
        }
        else if (state->board[num] != NONE)
        {
            printf("Cell occupied. Please enter a different cell\n");
            continue;
        }
        else
            break;
    }
    state->board[num] = state->turn;

    if (state->turn == X)
        state->turn = O;
    else
        state->turn = X;
    return;
}

Player check_won(GameState *state)
{
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            Player currentCell = state->board[i * BOARD_HEIGHT + j];
            if (currentCell == NONE)
                continue;

            // check down
            for (int k = 0; k < IN_A_ROW; k++)
            {
                if (k + i >= BOARD_HEIGHT)
                    break;
                if (state->board[(k + i) * BOARD_HEIGHT + j] != currentCell)
                    break;
                if (!(k + 1 < IN_A_ROW))
                    return currentCell;
            }
            // check right
            for (int k = 0; k < IN_A_ROW; k++)
            {
                if (k + j >= BOARD_WIDTH)
                    break;
                if (state->board[i * BOARD_HEIGHT + k + j] != currentCell)
                    break;
                if (!(k + 1 < IN_A_ROW))
                    return currentCell;
            }
            // check diagonal right
            for (int k = 0; k < IN_A_ROW; k++)
            {
                if (k + j >= BOARD_WIDTH || k + i >= BOARD_WIDTH)
                    break;
                if (state->board[(k + i) * BOARD_HEIGHT + k + j] != currentCell)
                    break;
                if (!(k + 1 < IN_A_ROW))
                    return currentCell;
            }
            // check diagonal left
            for (int k = 0; k < IN_A_ROW; k++)
            {
                if (j - k < 0)
                    break;
                int the_construct = (i + k) * BOARD_HEIGHT + (j - k);
                if (the_construct < 0 || the_construct > BOARD_CELLS)
                    break;
                if (state->board[the_construct] != currentCell)
                    break;
                if (!(k + 1 < IN_A_ROW))
                    return currentCell;
            }
        }
    }
    return NONE;
}

int check_room_on_board(GameState *state)
{
    for (unsigned int i = 0; i < BOARD_CELLS; i++)
    {
        if (state->board[i] == NONE)
            return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 1 && argc != 4)
    {
        printf("Usage: ttt [<width> <height> <in a row>]");
        exit(1);
    }
    else if (argc == 4)
    {
        BOARD_WIDTH = strtolong(argv[1]);
        BOARD_HEIGHT = strtolong(argv[2]);
        IN_A_ROW = strtolong(argv[3]);
    }
    GameState *state = empty_state(X);
    while (1)
    {
        print_board(state);
        make_move_from_input(state);
        Player won = check_won(state);
        if (won != NONE)
        {
            if (won == X)
                printf("X won!\n");
            else
                printf("O won!\n");
            break;
        }
        else if (!check_room_on_board(state))
        {
            printf("Noone won!\n");
            break;
        }
    }
    printf("Final board position:\n");
    print_board(state);
    free(state);
}
