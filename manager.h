#include "position.h"

/**
 * Parses x and y values into a move
 * @param x x value of move
 * @param y y value of move
 * @return move
 */
uint64_t parse_input(int x, int y);

/**
 * Parses a string with format:
 * "a1b2c3"
 * Assumes standard starting position
 * @param string string for game
 * @return the game state after moves, or last legal move
 */
Game parse_game(char *string);