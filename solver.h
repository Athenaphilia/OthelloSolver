#include "position.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct Node {
    Game game; // representation of the game state
    struct Node *parent;
    struct Node **children; // pointers to all the child nodes
    uint64_t *legal_moves;  // legal moves respective to children
    int num_legal;          // number of legal moves
    uint64_t visits;
    double value;
    bool end;
} Node;

/**
 * Shuffles the array given
 * Used to pick random moves in an easier manner
 * Randomizes move order while stopping duplicates
 * @param legal_moves moves to randomize
 * @param num_legal size of array to randomize
 */
void shuffle_moves(uint64_t *legal_moves, int num_legal);

/**
 * Unimplemented function
 * Selects best scoring child based on function
 * @param node node to search
 * @return best child
 */
Node *select_child(Node *node);

/**
 * Gets the mcts score from a terminal game state
 * @param game state to check
 * @param optimizer player to get score for
 * @return mcts score of state
 */
double get_score_from_state(Game game, int optimizer);

/**
 * Propagates a mcts score up a tree
 * @param node node to propagate from
 * @param result mcts result to propagate
 */
void backpropogate(Node *node, double result);

/**
 * Expands one child from one node
 * Goes in order of legal_moves
 * @param node node to expand
 * @param optimizer for scoring if expansion ends in terminal state
 * @return 0 if child has legal moves, 1 if terminal state
 */
int expand_node(Node *node, int optimizer);

/**
 * Frees a tree recursively
 * Cleans up dangling references
 * @param root root of tree to free
 */
void free_tree(Node *root);