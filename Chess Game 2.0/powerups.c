#include "powerups.h"
#include "board.h"
#include "moves.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

extern int player_powerups[2][5];

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
    } else {
        printf("Move too far for boosted move!\n");
    }
}

void activate_power_up(PowerUp power) {
    int idx = (current_player == 'W') ? 0 : 1;
    if (player_powerups[idx][power] > 0) {
        printf("Power-up already available.\n");
    } else {
        player_powerups[idx][power] = 1;
        printf("Power-up added!\n");
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
            } else {
                printf("First move invalid!\n");
                break;
            }
            if (parse_position(from + 2, &fromX, &fromY) && parse_position(to + 2, &toX, &toY) &&
                is_valid_move(fromX, fromY, toX, toY, current_player)) {
                move_piece(fromX, fromY, toX, toY);
                print_board();
            } else {
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
