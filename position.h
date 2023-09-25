#include <stdint.h>
#define BOARD_SIZE 8
#define NUM_SQUARES (BOARD_SIZE * BOARD_SIZE)

// Define bitboard structures for black and white pieces
typedef struct Bitboard {
    uint64_t black;
    uint64_t white;
} Bitboard;

/**
 * @return a board of the starting
 */
Bitboard initialize_board();

/**
 * Checks a specified direction for flips
 * Helper function for generate_legal_moves()
 * @param board current board
 * @param player current player
 * @param square the move to check
 * @param dx column offset
 * @param dy row offset
 * @return a mask of the flipped pieces or 0 for no flips
 */
uint64_t check_direction(Bitboard board, int player, int square, int dx, int dy);

/**
 * Fills an array with all the legal moves in a position
 * Extra space is filled with 0's
 * @param board current board
 * @param player current player
 * @param legal_moves array to fill with legal moves
 */
void generate_legal_moves(Bitboard board, int player, uint64_t legal_moves[34]);

/**
 * Gets one bitmask for legal moves from an array of moves
 * @param legal_moves an array of legal moves with uninitialized elements set to 0
 * @return a bitmask with all legal moves
 */
uint64_t generate_int_moves(uint64_t legal_moves[34]);

/**
 * Calculates the flips needed in a certain direction
 * Helper function for make_move()
 * @param player_pieces the current player's pieces
 * @param opponent_pieces the opponent's pieces
 * @param move the move to check for flips
 * @param dy row offset
 * @param dx column offset
 * @return bitmask of flips in the direction
 */
uint64_t calculate_flips_direction(uint64_t player_pieces, uint64_t opponent_pieces, uint64_t move, int dy, int dx);

/**
 * Makes a move and flips the required pieces
 * @param board current board
 * @param player current player
 * @param move move to play (MUST BE LEGAL)
 * @return updated board
 */
Bitboard make_move(Bitboard board, int player, uint64_t move);

/**
 * Checks a board for it's state
 * @param board current board
 * @param player current player
 * @return 0: continue 1: stop playing 2: current player passes
 */
int find_state(Bitboard board, int player);

/**
 * Displays the current board with legal moves shown
 * @param board current board
 * @param legal_moves legal moves bitmask
 */
void display_board(Bitboard board, uint64_t legal_moves);

/**
 * Counts the pieces of a player
 * @param board current board
 * @param player player to count
 * @return the number of pieces belonging to a player
 */
int count_pieces(Bitboard board, int player);
