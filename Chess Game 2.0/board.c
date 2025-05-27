#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char board[SIZE][SIZE];
char current_player = 'W';

void initialize_board() {
    char initial[SIZE][SIZE] = {
        {'r','n','b','q','k','b','n','r'},
        {'p','p','p','p','p','p','p','p'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'P','P','P','P','P','P','P','P'},
        {'R','N','B','Q','K','B','N','R'}
    };
    memcpy(board, initial, sizeof(initial));
}

void print_board() {
    printf("\n    a b c d e f g h\n\n");
    for (int row = 0; row < SIZE; row++) {
        printf("%d   ", 8 - row);
        for (int col = 0; col < SIZE; col++) {
            printf("%c ", board[row][col]);
        }
        printf("  %d\n", 8 - row);
    }
    printf("\n    a b c d e f g h\n\n");
}

void move_piece(int fromX, int fromY, int toX, int toY) {
    char piece = board[fromY][fromX];
    char captured = board[toY][toX];
    if (captured != '.') {
        printf("Captured %c!\n", captured);
    }
    board[toY][toX] = piece;
    board[fromY][fromX] = '.';
    if (tolower(piece) == 'p' &&
        ((current_player == 'W' && toY == 0) || (current_player == 'B' && toY == 7))) {
        char promotion;
        do {
            printf("Promote pawn to (Q/R/B/N): ");
            scanf(" %c", &promotion);
            promotion = (current_player == 'W') ? toupper(promotion) : tolower(promotion);
        } while (strchr("QRBNqrbn", promotion) == NULL);
        board[toY][toX] = promotion;
    }
}
