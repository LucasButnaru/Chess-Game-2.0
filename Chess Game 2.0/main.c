#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 8

char board[SIZE][SIZE];
char current_player = 'W'; // 'W' for white, 'B' for black

void initialize_board() {
    char initial[SIZE][SIZE] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    memcpy(board, initial, sizeof(initial));
}

void print_board() {
    printf("\n    a b c d e f g h\n");
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d   ", 8 - i);
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("  %d\n", 8 - i);
    }
    printf("\n");
    printf("    a b c d e f g h\n");
}

bool parse_position(char* pos, int* x, int* y) {
    if (strlen(pos) != 2 || pos[0] < 'a' || pos[0] > 'h' || pos[1] < '1' || pos[1] > '8') {
        return false;
    }
    *x = pos[0] - 'a';
    *y = 8 - (pos[1] - '0');
    return true;
}

bool is_path_clear(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    int x_inc = dx == 0 ? 0 : (dx > 0 ? 1 : -1);
    int y_inc = dy == 0 ? 0 : (dy > 0 ? 1 : -1);

    for (int i = 1; i < steps; i++) {
        int x = x1 + x_inc * i;
        int y = y1 + y_inc * i;
        if (board[y][x] != '.') return false;
    }
    return true;
}

bool is_valid_pawn_move(int x1, int y1, int x2, int y2, char player) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    char dest = board[y2][x2];

    if (player == 'W') {
        if (dx == 0 && dest == '.' && dy == -1) return true;
        if (y1 == 6 && dx == 0 && dy == -2 && dest == '.' && board[y1 - 1][x1] == '.') return true;
        if (abs(dx) == 1 && dy == -1 && dest != '.' && islower(dest)) return true;
    }
    else {
        if (dx == 0 && dest == '.' && dy == 1) return true;
        if (y1 == 1 && dx == 0 && dy == 2 && dest == '.' && board[y1 + 1][x1] == '.') return true;
        if (abs(dx) == 1 && dy == 1 && dest != '.' && isupper(dest)) return true;
    }
    return false;
}

bool is_valid_knight_move(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

bool is_valid_bishop_move(int x1, int y1, int x2, int y2) {
    if (abs(x2 - x1) != abs(y2 - y1)) return false;
    return is_path_clear(x1, y1, x2, y2);
}

bool is_valid_rook_move(int x1, int y1, int x2, int y2) {
    if (x1 != x2 && y1 != y2) return false;
    return is_path_clear(x1, y1, x2, y2);
}

bool is_valid_queen_move(int x1, int y1, int x2, int y2) {
    return is_valid_rook_move(x1, y1, x2, y2) || is_valid_bishop_move(x1, y1, x2, y2);
}

bool is_valid_king_move(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    return dx <= 1 && dy <= 1;
}

bool is_valid_move(int x1, int y1, int x2, int y2, char player) {
    if (x1 < 0 || x1 >= SIZE || y1 < 0 || y1 >= SIZE ||
        x2 < 0 || x2 >= SIZE || y2 < 0 || y2 >= SIZE) return false;

    char piece = board[y1][x1];
    if (piece == '.' || (player == 'W' && islower(piece)) || (player == 'B' && isupper(piece)))
        return false;

    char dest = board[y2][x2];
    if (dest != '.' && ((player == 'W' && !islower(dest)) || (player == 'B' && !isupper(dest))))
        return false;

    switch (tolower(piece)) {
    case 'p': return is_valid_pawn_move(x1, y1, x2, y2, player);
    case 'n': return is_valid_knight_move(x1, y1, x2, y2);
    case 'b': return is_valid_bishop_move(x1, y1, x2, y2);
    case 'r': return is_valid_rook_move(x1, y1, x2, y2);
    case 'q': return is_valid_queen_move(x1, y1, x2, y2);
    case 'k': return is_valid_king_move(x1, y1, x2, y2);
    default: return false;
    }
}

void promote_pawn(int x, int y) {
    char piece;
    do {
        printf("Promote pawn to (Q/R/B/N): ");
        scanf(" %c", &piece); // Space before %c to skip whitespace
        piece = toupper(piece);
        if (current_player == 'B') {
            piece = tolower(piece);
        }
    } while (strchr("QRBNqrbn", piece) == NULL);

    board[y][x] = piece;
}

void move_piece(int x1, int y1, int x2, int y2) {
    char piece = board[y1][x1];
    char captured = board[y2][x2];

    // Handle capture
    if (captured != '.') {
        printf("Captured %c!\n", captured);
    }

    board[y2][x2] = piece;
    board[y1][x1] = '.';

    if (tolower(piece) == 'p' && ((current_player == 'W' && y2 == 0) || (current_player == 'B' && y2 == 7))) {
        promote_pawn(x2, y2);
    }
}


int main() {
    initialize_board();
    print_board();

    char from[3], to[3];
    while (1) {
        printf("%s's turn. Enter move (from to) or 'q' to quit: ", current_player == 'W' ? "White" : "Black");
        if (scanf("%2s", from) != 1 || from[0] == 'q') break;
        if (scanf("%2s", to) != 1) break;

        int x1, y1, x2, y2;
        if (!parse_position(from, &x1, &y1) || !parse_position(to, &x2, &y2)) {
            printf("Invalid positions!\n");
            continue;
        }

        if (is_valid_move(x1, y1, x2, y2, current_player)) {
            move_piece(x1, y1, x2, y2);
            current_player = (current_player == 'W') ? 'B' : 'W';
            print_board();
        }
        else {
            printf("Invalid move!\n");
        }
    }
    return 0;
}
