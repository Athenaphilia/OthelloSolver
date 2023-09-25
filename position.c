#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 8
#define NUM_SQUARES (BOARD_SIZE * BOARD_SIZE)

// Define bitboard structures for black and white pieces
typedef struct {
    uint64_t black;
    uint64_t white;
} Bitboard;

// Initialize a bitboard representing the starting position
Bitboard initialize_board() {
    Bitboard board;
    board.black = 0x0000000810000000ULL; // Initial black pieces
    board.white = 0x0000001008000000ULL; // Initial white pieces
    return board;
}

void print_bits(unsigned long n) {
    unsigned long i;
    i = 1UL << (sizeof(n) * CHAR_BIT - 1);
    while (i > 0) {
        if (n & i)
            printf("1");
        else
            printf("0");
        i >>= 1;
    }
}

// Function to check if a move is valid and return flipped pieces
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

// Function to generate an array of legal moves padded with zeros
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
            int valid = 0;

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

// Function to generate legal moves for a given player
uint64_t generate_int_moves(Bitboard board, int player) {
    uint64_t legal_moves = 0ULL;

    uint64_t player_pieces = (player == 1) ? board.black : board.white;
    uint64_t opponent_pieces = (player == 1) ? board.white : board.black;

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
                    int x = i % BOARD_SIZE;
                    int y = i / BOARD_SIZE;

                    uint64_t flips = 0ULL;
                    int valid = 0;

                    // Move along the current direction
                    x += dx;
                    y += dy;
                    int index = x + y * BOARD_SIZE;

                    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                        uint64_t square = 1ULL << index;

                        if (opponent_pieces & square) {
                            flips |= square;
                            x += dx;
                            y += dy;
                            index = x + y * BOARD_SIZE;

                            // Continue checking for flips
                            while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                                square = 1ULL << index;

                                if (player_pieces & square) {
                                    valid = 1;
                                    break;
                                } else if (opponent_pieces & square) {
                                    flips |= square;
                                } else {
                                    break;
                                }

                                x += dx;
                                y += dy;
                                index = x + y * BOARD_SIZE;
                            }
                        }
                    }

                    if (valid && flips)
                        legal_moves |= (1ULL << i);
                }
            }
        }
    }

    return legal_moves;
}

uint64_t calculate_flips_direction(uint64_t player_pieces, uint64_t opponent_pieces, uint64_t move, int row_offset, int col_offset) {
    uint64_t flipped = 0;
    uint64_t current_square = move;

    // Iterate in the specified direction
    for (int i = 0; i < BOARD_SIZE; i++) {
        current_square = (row_offset > 0) ? current_square << BOARD_SIZE : current_square;
        current_square = (row_offset < 0) ? current_square >> BOARD_SIZE : current_square;
        current_square = (col_offset > 0) ? current_square << 1 : current_square;
        current_square = (col_offset < 0) ? current_square >> 1 : current_square;

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

// Function to make a move and flip the required pieces
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

    player_pieces |= flips;
    opponent_pieces &= ~flips;
    board.black = (player == 1) ? player_pieces : opponent_pieces;
    board.white = (player == 1) ? opponent_pieces : player_pieces;

    return board;
}

// Function to display the current board state
void display_board(Bitboard board) {
    printf("  a b c d e f g h\n");
    for (int y = 0; y < BOARD_SIZE; y++) {
        printf("%d ", y + 1);
        for (int x = 0; x < BOARD_SIZE; x++) {
            int index = x + y * BOARD_SIZE;
            uint64_t square_mask = 1ULL << index;

            if (board.black & square_mask) {
                printf("B ");
            } else if (board.white & square_mask) {
                printf("W ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

// Function to count the number of pieces for each player
void count_pieces(Bitboard board, int *black_count, int *white_count) {
    *black_count = __builtin_popcountll(board.black);
    *white_count = __builtin_popcountll(board.white);
}

int main() {
    Bitboard board = initialize_board();
    int player = 1; // Assume black starts

    // Main game loop
    while (1) {
        // Display the current board
        display_board(board);

        uint64_t legal_moves = generate_int_moves(board, player);

        // Check for game over or no legal moves
        if (legal_moves == 0ULL) {
            // Implement endgame logic here
            int black_count, white_count;
            count_pieces(board, &black_count, &white_count);

            if (black_count > white_count) {
                printf("Player B wins with %d-%d!\n", black_count, white_count);
            } else if (white_count > black_count) {
                printf("Player W wins with %d-%d!\n", white_count, black_count);
            } else {
                printf("It's a draw! %d-%d\n", black_count, white_count);
            }
            break;
        }

        printf("Player %c's turn (Enter your move in the format 'xy', e.g., 'c3'): ", (player == 1) ? 'B' : 'W');

        char move_input[3];
        scanf("%s", move_input);

        // Convert the input to board coordinates (e.g., 'c3' to index 34)
        int x = move_input[0] - 'a';
        int y = move_input[1] - '1';
        int move_index = x + y * BOARD_SIZE;
        uint64_t chosen_move = 1ULL << move_index;

        // Check if the chosen move is legal
        if (legal_moves & chosen_move) {
            // Make the move and update the board
            board = make_move(board, player, chosen_move);

            // Switch to the other player
            player = 3 - player; // Toggle between player 1 and 2
        } else {
            printf("Invalid move. Please try again.\n");
        }
    }

    // Display the final board and determine the winner
    display_board(board);

    return 0;
}
