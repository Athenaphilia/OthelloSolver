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

    // main game loop
    while (true) {

        // get the legal moves for the position
        uint64_t legal_move_array[MAX_LEGAL_MOVES];
        generate_legal_moves(game, legal_move_array);
        uint64_t legal_moves = generate_int_moves(legal_move_array);

        int state = find_state(game);
        if (state == 1) {
            // game over
            int count_b = count_pieces(game, 1);
            int count_w = count_pieces(game, 2);
            display_board(game, legal_moves);
            if (count_b > count_w) {
                printf("Player B wins with %i-%i\n", count_b, count_w);
            } else if (count_b < count_w) {
                printf("Player W wins with %i-%i\n", count_w, count_b);
            } else {
                printf("It's a draw!");
            }
            break;
        } else if (state == 2) {
            // set the player correctly
            game.player = 3 - game.player;
            continue;
        }

        display_board(game, legal_moves);

        Node *root = initialize_root(game);
        start = clock();
        int best_move = monte_carlo_tree_search(root, UCB_C, root->game.player, budget, debug);
        end = clock();
        uint64_t move = legal_move_array[best_move];
        free_tree(root);

        if (debug) {
            printf("Time: %lu\n", (CLOCKS_PER_SEC) * (end - start));
        }
        if (move & legal_moves) {
            game = make_move(game, move);
        } else {
            printf("Invalid move\n");
        }
    }

    return 0;
}
