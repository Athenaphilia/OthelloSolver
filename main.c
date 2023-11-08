#include "position.h"
#include "solver.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {

    srand(time(NULL));

    double UCB_C = 1.5;    // upper confidence bound constant
    int budget = 10000;    // budget for monte-carlo tree search
    bool debug = false;    // true for showing debug info like time and node count
    char player1 = 'h';    // first player is human or machine
    char player2 = 'h';    // second player is human or machine
    bool lerping = false;  // whether to linear interpolate ucb constants over the game
    double v0 = 2, v1 = 1; // if we are lerping, what two values should be used

    // check for flags
    if (argc > 1) {
        for (int arg = 1; arg < argc; arg++) {
            if (test_flag(argv[arg], "-d")) {
                debug = true;
            } else if (test_flag(argv[arg], "-b")) {
                budget = atoi(argv[arg + 1]);
            } else if (test_flag(argv[arg], "-u")) {
                UCB_C = atof(argv[arg + 1]);
            } else if (test_flag(argv[arg], "-p")) {
                player1 = argv[arg + 1][0];
                player2 = argv[arg + 1][1];
            } else if (test_flag(argv[arg], "-l")) {
                lerping = true;
                v0 = atof(argv[arg + 1]);
                v1 = atof(argv[arg + 2]);
            }
        }
    }

    // set up variables we are going to use
    clock_t start, end;             // used for timing
    Game game = initialize_board(); // get starting position
    int turns_passed = 0;           // number of turns passed
    char current_player = player1;  // whether the current player is machine or human

    // main game loop
    while (true) {

        // get the legal moves for the position
        uint64_t legal_move_array[MAX_LEGAL_MOVES];
        generate_legal_moves(game, legal_move_array);
        uint64_t legal_moves = generate_int_moves(legal_move_array);

        // find if state is game over or pass
        int state = find_state(game);
        if (state == 1) {
            // game over
            // count the pieces for each player
            int count_b = count_pieces(game, 1);
            int count_w = count_pieces(game, 2);
            display_board(game, legal_moves);
            if (count_b > count_w) {
                // black wins
                printf("Player B wins with %i-%i\n", count_b, count_w);
            } else if (count_b < count_w) {
                // white wins
                printf("Player W wins with %i-%i\n", count_w, count_b);
            } else {
                // draw
                printf("It's a draw!");
            }
            break;
        } else if (state == 2) {
            // current player passes
            // set the player correctly
            game.player = 3 - game.player;
            current_player = (current_player == player1) ? player2 : player1;
            continue;
        }

        display_board(game, legal_moves);

        // choose who to get the move from
        uint64_t move;
        if (current_player == 'm') {
            // machine move

            if (lerping) {
                // lerp if we are lerping
                UCB_C = lerp(v0, v1, (double)turns_passed / NUM_SQUARES);
            }
            // initialize the root to the game state
            Node *root = initialize_root(game);

            // start the mcts
            start = clock();
            int best_move = monte_carlo_tree_search(root, UCB_C, root->game.player, budget, debug);
            end = clock();

            // get the best move
            move = legal_move_array[best_move];
            // free the tree
            free_tree(root);
            if (debug) {
                // if debugging, print the time
                printf("Time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
            }
        } else {
            // human move

            // get the move from stdin
            printf("Player %c's turn (Enter your move in the format 'xy', e.g., 'c3'): ", (game.player == 1) ? 'B' : 'W');
            char move_input[3];
            scanf("%s", move_input);

            // Convert the input to board coordinates (e.g., 'c3' to index 34)
            int x = move_input[0] - 'a';
            int y = move_input[1] - '1';
            int move_index = x + y * BOARD_SIZE;
            move = 1ULL << move_index;
        }

        if (move & legal_moves) {
            game = make_move(game, move);
            // switch players
            current_player = (current_player == player1) ? player2 : player1;
            if (turns_passed < NUM_SQUARES) {
                turns_passed++;
            }
        } else {
            printf("Invalid move\n");
        }
    }

    return 0;
}
