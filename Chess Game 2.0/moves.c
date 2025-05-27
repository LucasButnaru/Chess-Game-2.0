#include "moves.h"
#include "board.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static bool is_path_clear(int fromX, int fromY, int toX, int toY) {
    int dx = (toX - fromX) != 0 ? (toX - fromX) / abs(toX - fromX) : 0;
    int dy = (toY - fromY) != 0 ? (toY - fromY) / abs(toY - fromY) : 0;

    int x = fromX + dx, y = fromY + dy;
    while (x != toX || y != toY) {
        if (board[y][x] != '.') return false;
        x += dx;
        y += dy;
    }
    return true;
}

static bool is_valid_pawn_move(int fromX, int fromY, int toX, int toY, char player) {
    int dx = toX - fromX;
    int dy = toY - fromY;
    char dest = board[toY][toX];

    if (player == 'W') {
        if (dx == 0 && dy == -1 && dest == '.') return true;
        if (fromY == 6 && dx == 0 && dy == -2 && dest == '.' && board[fromY - 1][fromX] == '.') return true;
        if (abs(dx) == 1 && dy == -1 && dest != '.' && islower(dest)) return true;
    } else {
        if (dx == 0 && dy == 1 && dest == '.') return true;
        if (fromY == 1 && dx == 0 && dy == 2 && dest == '.' && board[fromY + 1][fromX] == '.') return true;
        if (abs(dx) == 1 && dy == 1 && dest != '.' && isupper(dest)) return true;
    }
    return false;
}

static bool is_valid_knight_move(int fromX, int fromY, int toX, int toY) {
    int dx = abs(toX - fromX), dy = abs(toY - fromY);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

static bool is_valid_bishop_move(int fromX, int fromY, int toX, int toY) {
    return abs(toX - fromX) == abs(toY - fromY) && is_path_clear(fromX, fromY, toX, toY);
}

static bool is_valid_rook_move(int fromX, int fromY, int toX, int toY) {
    return (fromX == toX || fromY == toY) && is_path_clear(fromX, fromY, toX, toY);
}

static bool is_valid_queen_move(int fromX, int fromY, int toX, int toY) {
    return is_valid_rook_move(fromX, fromY, toX, toY) || is_valid_bishop_move(fromX, fromY, toX, toY);
}

static bool is_valid_king_move(int fromX, int fromY, int toX, int toY) {
    return abs(toX - fromX) <= 1 && abs(toY - fromY) <= 1;
}

void find_king(char player, int* x, int* y) {
    char king = (player == 'W') ? 'K' : 'k';
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == king) {
                *x = col;
                *y = row;
                return;
            }
        }
    }
    *x = *y = -1;
}

bool is_square_under_attack(int x, int y, char player) {
    char opponent = (player == 'W') ? 'B' : 'W';
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (is_valid_move(col, row, x, y, opponent)) {
                return true;
            }
        }
    }
    return false;
}

bool is_valid_move(int fromX, int fromY, int toX, int toY, char player) {
    if (fromX < 0 || fromX >= SIZE || fromY < 0 || fromY >= SIZE ||
        toX < 0 || toX >= SIZE || toY < 0 || toY >= SIZE) return false;

    char piece = board[fromY][fromX];
    if (piece == '.' || (player == 'W' && islower(piece)) || (player == 'B' && isupper(piece)))
        return false;

    char dest = board[toY][toX];
    if (dest != '.' && ((player == 'W' && isupper(dest)) || (player == 'B' && islower(dest))))
        return false;

    bool valid = false;
    switch (tolower(piece)) {
        case 'p': valid = is_valid_pawn_move(fromX, fromY, toX, toY, player); break;
        case 'n': valid = is_valid_knight_move(fromX, fromY, toX, toY); break;
        case 'b': valid = is_valid_bishop_move(fromX, fromY, toX, toY); break;
        case 'r': valid = is_valid_rook_move(fromX, fromY, toX, toY); break;
        case 'q': valid = is_valid_queen_move(fromX, fromY, toX, toY); break;
        case 'k': valid = is_valid_king_move(fromX, fromY, toX, toY); break;
    }

    if (!valid) return false;

    char tmp_from = board[fromY][fromX];
    char tmp_to = board[toY][toX];
    board[toY][toX] = tmp_from;
    board[fromY][fromX] = '.';

    int kingX, kingY;
    find_king(player, &kingX, &kingY);
    bool in_check = is_square_under_attack(kingX, kingY, player);

    board[fromY][fromX] = tmp_from;
    board[toY][toX] = tmp_to;

    return !in_check;
}

bool is_checkmate(char player) {
    int kingX, kingY;
    find_king(player, &kingX, &kingY);
    if (!is_square_under_attack(kingX, kingY, player)) return false;

    for (int fromY = 0; fromY < SIZE; fromY++) {
        for (int fromX = 0; fromX < SIZE; fromX++) {
            char piece = board[fromY][fromX];
            if ((player == 'W' && isupper(piece)) || (player == 'B' && islower(piece))) {
                for (int toY = 0; toY < SIZE; toY++) {
                    for (int toX = 0; toX < SIZE; toX++) {
                        if (is_valid_move(fromX, fromY, toX, toY, player)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}
