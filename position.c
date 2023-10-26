#include "position.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define NOT_LEFT_COLUMN 0xFEFEFEFEFEFEFEFEULL
#define NOT_RIGHT_COLUMN 0x7F7F7F7F7F7F7F7FULL

Game initialize_board() {
    Game game;
    game.black = 0x0000000810000000ULL; // Initial black pieces
    game.white = 0x0000001008000000ULL; // Initial white pieces
    game.player = 1;
    return game;
}

uint64_t check_direction(Game game, int square, int dx, int dy) {
    uint64_t player_pieces = (game.player == 1) ? game.black : game.white;
    uint64_t opponent_pieces = (game.player == 1) ? game.white : game.black;

    int x = square % BOARD_SIZE;
    int y = square / BOARD_SIZE;
    uint64_t flips = 0ULL;

    // Move along the current direction
    x += dx;
    y += dy;
    int index = x + y * BOARD_SIZE;

    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
        uint64_t square_mask = 1ULL << index;

        if (opponent_pieces & square_mask) {
            flips |= square_mask;
            x += dx;
            y += dy;
            index = x + y * BOARD_SIZE;

            // Continue checking for flips
            while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                square_mask = 1ULL << index;

                if (player_pieces & square_mask) {
                    return flips;
                } else if (opponent_pieces & square_mask) {
                    flips |= square_mask;
                } else {
                    break;
                }

                x += dx;
                y += dy;
                index = x + y * BOARD_SIZE;
            }
        }
    }

    return 0ULL;
}

int generate_legal_moves(Game game, uint64_t *legal_moves) {
    int num_moves = 0;

    uint64_t player_pieces = (game.player == 1) ? game.black : game.white;
    uint64_t opponent_pieces = (game.player == 1) ? game.white : game.black;

    // Initialize the legal_moves array with zeros
    for (int i = 0; i < MAX_LEGAL_MOVES; i++) {
        legal_moves[i] = 0ULL;
    }

    // Iterate over all squares
    for (int i = 0; i < NUM_SQUARES; i++) {
        uint64_t square_mask = 1ULL << i;

        // Check if the square is empty
        if (!(player_pieces & square_mask) && !(opponent_pieces & square_mask)) {

            // Check in all eight directions for potential flips
            for (int dy = -1; dy < 2; dy++) {
                for (int dx = -1; dx < 2; dx++) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }

                    uint64_t flips = check_direction(game, i, dx, dy);

                    if (flips) {
                        if (num_moves < MAX_LEGAL_MOVES) {
                            legal_moves[num_moves] = square_mask;
                            num_moves++;
                        }
                    }
                }
            }
        }
    }
    return num_moves;
}

uint64_t generate_int_moves(uint64_t *legal_moves) {
    uint64_t move = 0ULL;
    for (int i = 0; i < MAX_LEGAL_MOVES; i++) {
        if (legal_moves[i] == 0ULL) {
            break;
        }
        move |= legal_moves[i];
    }
    return move;
}

uint64_t calculate_flips_direction(uint64_t player_pieces, uint64_t opponent_pieces, uint64_t move, int dy, int dx) {
    uint64_t flipped = 0;
    uint64_t current_square = move;

    // Iterate in the specified direction
    for (int i = 0; i < BOARD_SIZE; i++) {
        current_square = (dy > 0) ? current_square << BOARD_SIZE : current_square;
        current_square = (dy < 0) ? current_square >> BOARD_SIZE : current_square;
        current_square = (dx > 0) ? (current_square << 1 & NOT_LEFT_COLUMN) : current_square;
        current_square = (dx < 0) ? (current_square >> 1 & NOT_RIGHT_COLUMN) : current_square;

        // Check if the current square is out of bounds or empty
        if (!(current_square & (player_pieces | opponent_pieces))) {
            break;
        }

        // Check if the current square contains an opponent's piece
        if (current_square & opponent_pieces) {
            flipped |= current_square;
        } else {
            // If a player's piece is encountered, potential flips become actual flips
            return flipped;
        }
    }

    return 0; // No flips possible in this direction
}

Game make_move(Game game, uint64_t move) {

    // set the player's piece on the board
    if (game.player == 1) {
        game.black |= move;
    } else {
        game.white |= move;
    }

    // opponents
    uint64_t player_pieces = (game.player == 1) ? game.black : game.white;
    uint64_t opponent_pieces = (game.player == 1) ? game.white : game.black;

    // check all directions
    uint64_t flips = 0ULL;
    for (int dy = -1; dy < 2; dy++) {
        for (int dx = -1; dx < 2; dx++) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            flips |= calculate_flips_direction(player_pieces, opponent_pieces, move, dy, dx);
        }
    }

    // apply the flips
    player_pieces |= flips;
    opponent_pieces &= ~flips;
    game.black = (game.player == 1) ? player_pieces : opponent_pieces;
    game.white = (game.player == 1) ? opponent_pieces : player_pieces;
    game.player = 3 - game.player;

    return game;
}

int find_state(Game game) {

    // check if the board is full
    if (~(game.black | game.white) == 0ULL) {
        return 1; // game over
    }
    uint64_t legal_moves_player[MAX_LEGAL_MOVES];

    generate_legal_moves(game, legal_moves_player);
    if (generate_int_moves(legal_moves_player) == 0ULL) {
        uint64_t legal_moves_opponent[MAX_LEGAL_MOVES];
        game.player = 3 - game.player;
        generate_legal_moves(game, legal_moves_opponent);
        if (generate_int_moves(legal_moves_opponent) == 0ULL) {
            return 1; // double pass, game over
        }
        return 2; // pass
    }
    return 0; // continue
}

void display_board(Game game, uint64_t legal_moves) {
    printf("  a b c d e f g h\n");
    for (int y = 0; y < BOARD_SIZE; y++) {
        printf("%d ", y + 1);
        for (int x = 0; x < BOARD_SIZE; x++) {
            int index = x + y * BOARD_SIZE;
            uint64_t square_mask = 1ULL << index;

            if (game.black & square_mask) {
                printf("B ");
            } else if (game.white & square_mask) {
                printf("W ");
            } else if (legal_moves & square_mask) {
                printf("L ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

int count_pieces(Game game, int player) {
    return __builtin_popcountll((player == 1) ? game.black : game.white);
}