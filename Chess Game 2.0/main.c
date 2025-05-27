#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "utils.h"
#include "moves.h"
#include "powerups.h"

int player_powerups[2][5] = { {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1} };

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
            printf("Invalid input!\n");
            continue;
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
