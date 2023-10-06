#include "position.h"
#include "solver.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {

    srand(time(NULL));

    double UCB_C = 1.5;
    int budget = 10000;
    bool debug = false;

    if (argc > 1) {
        for (int arg = 1; arg < argc; arg++) {
            if (test_flag(argv[arg], "-d")) {
                debug = true;
            } else if (test_flag(argv[arg], "-b")) {
                budget = atoi(argv[arg + 1]);
                arg++;
            } else if (test_flag(argv[arg], "-u")) {
                UCB_C = atof(argv[arg + 1]);
                arg++;
            }
        }
    }

    clock_t start, end;
    Game game = initialize_board();

    // Main game loop
    while (1) {

        // find the legal moves
        uint64_t legal_moves_array[34];
        generate_legal_moves(game, legal_moves_array);
        uint64_t legal_moves = generate_int_moves(legal_moves_array);

        // Display the current board
        display_board(game, legal_moves);
        // Check for game over or no legal moves
        int state = find_state(game);
        if (state == 1) {

            // game over
            int black_count = count_pieces(game, 1);
            int white_count = count_pieces(game, 2);

            if (black_count > white_count) {
                printf("Player B wins with %d-%d!\n", black_count, white_count);
            } else if (white_count > black_count) {
                printf("Player W wins with %d-%d!\n", white_count, black_count);
            } else {
                printf("It's a draw! %d-%d\n", black_count, white_count);
            }
            break;
        } else if (state == 2) {
            // pass
            printf("Player %c passes\n", (game.player == 1) ? 'B' : 'W');
            game.player = (game.player == 1) ? 2 : 1;
            continue;
        }

        Node *root = initialize_root(game);
        start = clock();
        int best_move = monte_carlo_tree_search(root, UCB_C, root->game.player, budget, debug);
        end = clock();
        uint64_t chosen_move = root->legal_moves[best_move];
        free_tree(root);
        if (debug) {
            printf("Time: %lf\n", (((double)(end - start)) / CLOCKS_PER_SEC));
        }
        // Check if the chosen move is legal
        if (legal_moves & chosen_move) {
            // Make the move and update the board
            game = make_move(game, chosen_move);
        } else {
            printf("Invalid move. Please try again.\n");
        }
    }

    return 0;
}
