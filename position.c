#include "position.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Bitboard initialize_board() {
    Bitboard board;
    board.black = 0x0000000810000000ULL; // Initial black pieces
    board.white = 0x0000001008000000ULL; // Initial white pieces
    return board;
}

uint64_t check_direction(Bitboard board, int player, int square, int dx, int dy) {
    uint64_t player_pieces = (player == 1) ? board.black : board.white;
    uint64_t opponent_pieces = (player == 1) ? board.white : board.black;

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

void generate_legal_moves(Bitboard board, int player, uint64_t legal_moves[34]) {
    int num_moves = 0;

    uint64_t player_pieces = (player == 1) ? board.black : board.white;
    uint64_t opponent_pieces = (player == 1) ? board.white : board.black;

    // Initialize the legal_moves array with zeros
    for (int i = 0; i < 34; i++) {
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

                    uint64_t flips = check_direction(board, player, i, dx, dy);

                    if (flips) {
                        if (num_moves < 34) {
                            legal_moves[num_moves] = square_mask;
                            num_moves++;
                        }
                    }
                }
            }
        }
    }
}

uint64_t generate_int_moves(uint64_t legal_moves[34]) {
    uint64_t move = 0ULL;
    for (int i = 0; i < 34; i++) {
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
        current_square = (dx > 0) ? current_square << 1 : current_square;
        current_square = (dx < 0) ? current_square >> 1 : current_square;

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

Bitboard make_move(Bitboard board, int player, uint64_t move) {

    // set the player's piece on the board
    if (player == 1) {
        board.black |= move;
    } else {
        board.white |= move;
    }

    // opponents bitboard
    uint64_t player_pieces = (player == 1) ? board.black : board.white;
    uint64_t opponent_pieces = (player == 1) ? board.white : board.black;

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
    board.black = (player == 1) ? player_pieces : opponent_pieces;
    board.white = (player == 1) ? opponent_pieces : player_pieces;

    return board;
}

int find_state(Bitboard board, int player) {

    // check if the board is full
    if (~(board.black | board.white) == 0ULL) {
        return 1; // game over
    }
    uint64_t legal_moves_player[34];

    generate_legal_moves(board, player, legal_moves_player);
    if (generate_int_moves(legal_moves_player) == 0ULL) {
        uint64_t legal_moves_opponent[34];
        generate_legal_moves(board, (player == 1) ? 2 : 1, legal_moves_opponent);
        if (generate_int_moves(legal_moves_opponent) == 0ULL) {
            return 1; // double pass, game over
        }
        return 2; // pass
    }
    return 0; // continue
}

void display_board(Bitboard board, uint64_t legal_moves) {
    printf("  1 2 3 4 5 6 7 8\n");
    for (int y = 0; y < BOARD_SIZE; y++) {
        printf("%d ", y + 1);
        for (int x = 0; x < BOARD_SIZE; x++) {
            int index = x + y * BOARD_SIZE;
            uint64_t square_mask = 1ULL << index;

            if (board.black & square_mask) {
                printf("B ");
            } else if (board.white & square_mask) {
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

int count_pieces(Bitboard board, int player) {
    return __builtin_popcountll((player == 1) ? board.black : board.white);
}