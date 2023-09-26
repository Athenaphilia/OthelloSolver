#include "solver.h"
#include "position.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

// USC1 exploration constant
#define UCB1_C 1.0

typedef struct Node {
    Game game; // representation of the game state
    Node *parent;
    Node *children[34];                    // pointers to all the child nodes
    uint64_t legal_moves[MAX_LEGAL_MOVES]; // legal moves, should be shuffled
    uint64_t visits;
    double value;
} Node;
