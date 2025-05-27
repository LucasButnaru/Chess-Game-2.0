#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 8
#define POWER_UP_COUNT 5

typedef enum {
    TELEPORTATION,
    DOUBLE_MOVE,
    SWAP_PIECES,
    SHIELD,
    MOVE_BOOST
} PowerUp;


char board[SIZE][SIZE];
char current_player = 'W';
int player_powerups[2][POWER_UP_COUNT] = { {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1} };

void initialize_board(void);
void print_board(void);
bool parse_position(char* pos, int* x, int* y);
bool is_valid_move(int fromX, int fromY, int toX, int toY, char player);
bool is_checkmate(char player);
void move_piece(int fromX, int fromY, int toX, int toY);
void promote_pawn(int x, int y);
void find_king(char player, int* x, int* y);
bool is_square_under_attack(int x, int y, char player);
void power_up_triggered(void);
void use_teleportation(int fromX, int fromY, int toX, int toY);
void use_double_move(void);
void use_swap_pieces(int x1, int y1, int x2, int y2);
void use_shield(void);
void use_move_boost(int fromX, int fromY, int toX, int toY);



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


bool parse_position(char* pos, int* x, int* y) {
    if (strlen(pos) != 2 || pos[0] < 'a' || pos[0] > 'h' || pos[1] < '1' || pos[1] > '8') return false;
    *x = pos[0] - 'a'; *y = 8 - (pos[1] - '0');
    return true;
}


bool is_path_clear(int fromX, int fromY, int toX, int toY) {
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


void activate_power_up(PowerUp power) {
    int idx = (current_player == 'W') ? 0 : 1;
    if (player_powerups[idx][power] > 0) {
        printf("Power-up already available.\n");
    }
    else {
        player_powerups[idx][power] = 1;
        printf("Power-up added!\n");
    }
}


void use_teleportation(int fromX, int fromY, int toX, int toY) {
    int idx = (current_player == 'W') ? 0 : 1;
    if (player_powerups[idx][TELEPORTATION] <= 0) {
        printf("No teleportation power-up available!\n");
        return;
    }
    if (board[toY][toX] == '.') {
        move_piece(fromX, fromY, toX, toY);
        printf("Teleportation used!\n");
        player_powerups[idx][TELEPORTATION]--;
    }
}


void use_double_move(void) {
    int idx = (current_player == 'W') ? 0 : 1;
    if (player_powerups[idx][DOUBLE_MOVE] <= 0) {
        printf("No double move power-up available!\n");
        return;
    }
    printf("Double move activated!\n");
    player_powerups[idx][DOUBLE_MOVE]--;
}


void use_swap_pieces(int x1, int y1, int x2, int y2) {
    int idx = (current_player == 'W') ? 0 : 1;
    if (player_powerups[idx][SWAP_PIECES] <= 0) {
        printf("No swap power-up available!\n");
        return;
    }
    char tmp = board[y1][x1];
    board[y1][x1] = board[y2][x2];
    board[y2][x2] = tmp;
    printf("Pieces swapped!\n");
    player_powerups[idx][SWAP_PIECES]--;
}


void use_shield(void) {
    int idx = (current_player == 'W') ? 0 : 1;
    if (player_powerups[idx][SHIELD] <= 0) {
        printf("No shield power-up available!\n");
        return;
    }
    printf("Shield activated! Your king cannot be checked this turn.\n");
    player_powerups[idx][SHIELD]--;
}


void use_move_boost(int fromX, int fromY, int toX, int toY) {
    int idx = (current_player == 'W') ? 0 : 1;
    if (player_powerups[idx][MOVE_BOOST] <= 0) {
        printf("No move boost power-up available!\n");
        return;
    }
    if (abs(toX - fromX) <= 2 && abs(toY - fromY) <= 2) {
        move_piece(fromX, fromY, toX, toY);
        printf("Move boost activated!\n");
        player_powerups[idx][MOVE_BOOST]--;
    }
    else {
        printf("Move too far for boosted move!\n");
    }
}


void power_up_triggered() {
    int choice;
    printf("Choose a power-up to activate:\n");
    printf("1: Teleportation\n2: Double Move\n3: Swap Pieces\n4: Shield\n5: Move Boost\n");
    scanf("%d", &choice);

    char from[3], to[3];
    int fromX, fromY, toX, toY;

    switch (choice) {
    case 1:
        printf("Enter source and destination (e.g., e2 e4): ");
        scanf("%2s %2s", from, to);
        if (parse_position(from, &fromX, &fromY) && parse_position(to, &toX, &toY))
            use_teleportation(fromX, fromY, toX, toY);
        else
            printf("Invalid positions!\n");
        break;
    case 2:
        use_double_move();
        printf("Enter two moves (e.g., e2 e4 e7 e5): ");
        scanf("%2s %2s %2s %2s", from, to, from + 2, to + 2);
        if (parse_position(from, &fromX, &fromY) && parse_position(to, &toX, &toY) &&
            is_valid_move(fromX, fromY, toX, toY, current_player)) {
            move_piece(fromX, fromY, toX, toY);
            print_board();
        }
        else {
            printf("First move invalid!\n");
            break;
        }
        if (parse_position(from + 2, &fromX, &fromY) && parse_position(to + 2, &toX, &toY) &&
            is_valid_move(fromX, fromY, toX, toY, current_player)) {
            move_piece(fromX, fromY, toX, toY);
            print_board();
        }
        else {
            printf("Second move invalid!\n");
        }
        break;
    case 3:
        printf("Enter two positions to swap (e.g., b1 g8): ");
        scanf("%2s %2s", from, to);
        if (parse_position(from, &fromX, &fromY) && parse_position(to, &toX, &toY))
            use_swap_pieces(fromX, fromY, toX, toY);
        else
            printf("Invalid positions!\n");
        break;
    case 4:
        use_shield();
        break;
    case 5:
        printf("Enter boosted move (e.g., e2 e4): ");
        scanf("%2s %2s", from, to);
        if (parse_position(from, &fromX, &fromY) && parse_position(to, &toX, &toY))
            use_move_boost(fromX, fromY, toX, toY);
        else
            printf("Invalid positions!\n");
        break;
    default:
        printf("Invalid option!\n");
        break;
    }
}


bool is_valid_pawn_move(int fromX, int fromY, int toX, int toY, char player) {
    int dx = toX - fromX;
    int dy = toY - fromY;
    char dest = board[toY][toX];

    if (player == 'W') {
        if (dx == 0 && dy == -1 && dest == '.') return true;
        if (fromY == 6 && dx == 0 && dy == -2 && dest == '.' && board[fromY - 1][fromX] == '.') return true;
        if (abs(dx) == 1 && dy == -1 && dest != '.' && islower(dest)) return true;
    }
    else {
        if (dx == 0 && dy == 1 && dest == '.') return true;
        if (fromY == 1 && dx == 0 && dy == 2 && dest == '.' && board[fromY + 1][fromX] == '.') return true;
        if (abs(dx) == 1 && dy == 1 && dest != '.' && isupper(dest)) return true;
    }
    return false;
}


bool is_valid_knight_move(int fromX, int fromY, int toX, int toY) {
    int dx = abs(toX - fromX), dy = abs(toY - fromY);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}


bool is_valid_bishop_move(int fromX, int fromY, int toX, int toY) {
    return abs(toX - fromX) == abs(toY - fromY) && is_path_clear(fromX, fromY, toX, toY);
}


bool is_valid_rook_move(int fromX, int fromY, int toX, int toY) {
    return (fromX == toX || fromY == toY) && is_path_clear(fromX, fromY, toX, toY);
}


bool is_valid_queen_move(int fromX, int fromY, int toX, int toY) {
    return is_valid_rook_move(fromX, fromY, toX, toY) || is_valid_bishop_move(fromX, fromY, toX, toY);
}


bool is_valid_king_move(int fromX, int fromY, int toX, int toY) {
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

    // Simulate move
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


void promote_pawn(int x, int y) {
    char piece;
    do {
        printf("Promote pawn to (Q/R/B/N): ");
        scanf(" %c", &piece);
        piece = (current_player == 'W') ? toupper(piece) : tolower(piece);
    } while (strchr("QRBNqrbn", piece) == NULL);
    board[y][x] = piece;
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


int main() {
    initialize_board();
    print_board();
    char from[3], to[3];
    while (1) {
        printf("%s's turn. Enter move (from to), 'q' to quit, or 'p' for power-up: ", current_player == 'W' ? "White" : "Black");
        if (scanf("%2s", from) != 1 || from[0] == 'q') break;
        if (from[0] == 'p') {
            power_up_triggered();
            continue;
        }
        if (scanf("%2s", to) != 1) break;
        int fromX, fromY, toX, toY;
        if (!parse_position(from, &fromX, &fromY) || !parse_position(to, &toX, &toY)) {
            printf("Invalid input!\n"); continue;
        }
        if (is_valid_move(fromX, fromY, toX, toY, current_player)) {
            move_piece(fromX, fromY, toX, toY);
            print_board();
            char opponent = (current_player == 'W') ? 'B' : 'W';
            int kingX, kingY;
            find_king(opponent, &kingX, &kingY);
            if (is_square_under_attack(kingX, kingY, opponent)) {
                if (is_checkmate(opponent)) {
                    printf("Checkmate! %s wins!\n", current_player == 'W' ? "White" : "Black");
                    break;
                }
                else {
                    printf("Check!\n");
                }
            }
            current_player = opponent;
        }
        else {
            printf("Invalid move!\n");
        }
    }
    return 0;
}
