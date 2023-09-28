#ifndef POSITION_H
#define POSITION_H
#include <stdint.h>
#define BOARD_SIZE 8
#define NUM_SQUARES (BOARD_SIZE * BOARD_SIZE)
#define MAX_LEGAL_MOVES 34

// Define bitboard structures for black and white pieces
typedef struct Game {
    uint64_t black; // bitboard of black pieces
    uint64_t white; // bitboard of white pieces
    int player;     // current player
} Game;

/**
 * @return a board of the starting
 */
Game initialize_board();

/**
 * Checks a specified direction for flips
 * Helper function for generate_legal_moves()
 * @param game current board
 * @param square the move to check
 * @param dx column offset
 * @param dy row offset
 * @return a mask of the flipped pieces or 0 for no flips
 */
uint64_t check_direction(Game game, int square, int dx, int dy);

/**
 * Fills an array with all the legal moves in a position
 * Extra space is filled with 0's
 * @param game current board
 * @param legal_moves array to fill with legal moves
 * @return number of legal moves found
 */
int generate_legal_moves(Game game, uint64_t *legal_moves);

/**
 * Gets one bitmask for legal moves from an array of moves
 * @param legal_moves an array of legal moves with uninitialized elements set to 0
 * @return a bitmask with all legal moves
 */
uint64_t generate_int_moves(uint64_t *legal_moves);

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
 * Updates the player, but does not check for state
 * User should check if the next player passes and update accordingly
 * @param game current board
 * @param move move to play (MUST BE LEGAL)
 * @return updated board
 */
Game make_move(Game game, uint64_t move);

/**
 * Checks a board for it's state
 * @param game current board
 * @return 0: continue 1: stop playing 2: current player passes
 */
int find_state(Game game);

/**
 * Displays the current board with legal moves shown
 * @param game current board
 * @param legal_moves legal moves bitmask
 */
void display_board(Game game, uint64_t legal_moves);

/**
 * Counts the pieces of a player
 * @param game current board
 * @return the number of pieces belonging to a player
 */
int count_pieces(Game game, int player);

#endif