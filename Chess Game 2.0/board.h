#ifndef BOARD_H
#define BOARD_H

#define SIZE 8

extern char board[SIZE][SIZE];
extern char current_player;

void initialize_board(void);
void print_board(void);
void move_piece(int fromX, int fromY, int toX, int toY);

#endif
