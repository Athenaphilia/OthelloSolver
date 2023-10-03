#include "position.h"
#include "solver.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {

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

    /*
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

        printf("Player %c's turn (Enter your move in the format 'xy', e.g., '33'): ", (game.player == 1) ? 'B' : 'W');

        char move_input[3];
        scanf("%s", move_input);

        // Convert the input to board coordinates (e.g., '33' to index 34)
        int x = move_input[0] - '1';
        int y = move_input[1] - '1';
        int move_index = x + y * BOARD_SIZE;
        uint64_t chosen_move = 1ULL << move_index;

        // Check if the chosen move is legal
        if (legal_moves & chosen_move) {
            // Make the move and update the board
            game = make_move(game, chosen_move);
        } else {
            printf("Invalid move. Please try again.\n");
        }
    }
    */

    srand(time(NULL));

    Node *root = initialize_root();
    clock_t start, end;
    start = clock();
    int best_move = monte_carlo_tree_search(root, UCB_C, root->game.player, budget, debug);
    end = clock();
    if (debug) {
        printf("Time: %lf\n", (((double)(end - start)) / CLOCKS_PER_SEC));
    }
    printf("Best move: %i\n", best_move);
    free_tree(root);
    return 0;

    return 0;
}
