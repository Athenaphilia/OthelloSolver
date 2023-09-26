#include "solver.h"
#include "position.h"
#include <math.h>

// USC! exploration constant
#define UCB1_C 1.0

typedef struct Node {
    Game game;
    Node *parent;
    Node *children[34];
    uint64_t visits;
    double value;
} Node;