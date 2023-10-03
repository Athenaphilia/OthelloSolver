#include "manager.h"

uint64_t parse_input(int x, int y) {

    // parse into move index
    int move_index = x + y * BOARD_SIZE;

    // bitshift to get move
    uint64_t chosen_move = 1ULL << move_index;
    return chosen_move;
}

Game parse_game(char *string) {

    // assume starting position
    Game game = initialize_board();

    // find length of string
    int len = 0;
    while (string[len] != '\0') {
        len++;
    }
    // error check if string is not dividable in 2 chunks
    if (len % 2 != 1) {
        return game;
    }

    // parse the string in two char chunks
    for (int i = 0; i < len; i += 2) {

        // parse input into uint format
        int x = string[i] - 'a';
        int y = string[i + 1] - '1';
        uint64_t move = parse_input(x, y);

        // check if move is legal
        uint64_t legal_moves_array[MAX_LEGAL_MOVES]; // this is solely for generate_int_moves, maybe refactor?
        uint64_t legal_moves = generate_int_moves(generate_legal_moves(game, legal_moves_array));

        // invalid move, error
        if (!(legal_moves | move)) {
            return game;
        }

        game = make_move(game, move);
    }
}