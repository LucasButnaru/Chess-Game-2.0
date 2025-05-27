#ifndef MOVES_H
#define MOVES_H

#include <stdbool.h>

bool is_valid_move(int fromX, int fromY, int toX, int toY, char player);
bool is_checkmate(char player);
void find_king(char player, int* x, int* y);
bool is_square_under_attack(int x, int y, char player);

#endif
