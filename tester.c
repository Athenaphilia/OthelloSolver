#include "tester.h"
#include "position.h"
#include <stdio.h>
#include <time.h>

#define MAX_TEST_RUNS 1000000

bool test_move_generation_time() {
    Game game = initialize_board();
    uint64_t legal_moves[MAX_LEGAL_MOVES];
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int i = 0; i < MAX_TEST_RUNS; i++) {
        generate_legal_moves(game, legal_moves);
    }
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Move generation time: %f seconds\n", cpu_time_used);

    // Add your assertions here
    // For example, check if the number of legal moves is as expected

    return true;
}

bool test_game_play_time() {
    Game game = initialize_board();
    uint64_t legal_moves[MAX_LEGAL_MOVES];
    int state;

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    while ((state = find_state(game)) == 0) {
        generate_legal_moves(game, legal_moves);
        game = make_move(game, legal_moves[0]);
    }
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Game play time: %f seconds\n", cpu_time_used);

    // Add your assertions here
    // For example, check if the game state is as expected at the end

    return true;
}

int main() {
    if (test_move_generation_time() && test_game_play_time()) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return 0;
}
