#include <stdint.h>
#define BOARD_SIZE 8
#define NUM_SQUARES (BOARD_SIZE * BOARD_SIZE)

// Define bitboard structures for black and white pieces
typedef struct Bitboard {
    uint64_t black;
    uint64_t white;
} Bitboard;

// Initialize a bitboard representing the starting position
Bitboard initialize_board();

// Function to check if a move is valid and return flipped pieces
uint64_t check_direction(Bitboard board, int player, int square, int dx, int dy);

// Function to generate an array of legal moves padded with zeros
void generate_legal_moves(Bitboard board, int player, uint64_t legal_moves[34]);

// Function to generate legal moves for a given player
uint64_t generate_int_moves(uint64_t legal_moves[34]);

// Calculate the flips needed when a move is made
uint64_t calculate_flips_direction(uint64_t player_pieces, uint64_t opponent_pieces, uint64_t move, int dy, int dx);

// Function to make a move and flip the required pieces
Bitboard make_move(Bitboard board, int player, uint64_t move);

// Function to display the current board state
void display_board(Bitboard board, uint64_t legal_moves);

// Function to count the number of pieces for a player
int count_pieces(Bitboard board, int player);