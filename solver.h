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
    bool end; // this is a redundant variable (maybe provides speed up?), refactor later to remove
} Node;

/**
 * Finds if the node is a leaf
 * @param node node to check
 * @return if node is leaf
 */
bool is_leaf(Node *node);

/**
 * Finds the Upper Confidence Bound of a node
 * Uses a constant of 1.5
 * @param node node to find ucb1 for
 * @return ucb1 score
 */
double UCB1(Node *node);

/**
 * Shuffles the array given
 * Used to pick random moves in an easier manner
 * Randomizes move order while stopping duplicates
 * @param legal_moves moves to randomize
 * @param num_legal size of array to randomize
 */
void shuffle_moves(uint64_t *legal_moves, int num_legal);

/**
 * Gets a random move in a range
 * @param num_legal the number of legal moves
 * @return a random index in for the legal moves
 */
int random_move(int num_legal);

/**
 * Selects best scoring child based on function
 * Currently using ucb1
 * @param node node to search
 * @return best child
 */
Node *select_child(Node *node);

/**
 * Selects the best node in a tree based on function
 * Currently using ucb1 with greedy algorithm
 * @param root root of tree to search
 * @return the best node in the tree
 */
Node *select_best_node(Node *root);

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
 * @return the expanded node
 */
Node *expand_node(Node *node);

/**
 * Simulates a game down to a terminal state using a function
 * Currently uses random selection
 * @param game game to simulate
 * @param optimizer which player mcts is optimizing for
 * @return the score of the terminal game state
 */
double simulate(Game game, int optimizer);

/**
 * Frees a tree recursively
 * Cleans up dangling references
 * @param root root of tree to free
 */
void free_tree(Node *root);

/**
 * Finds the best move according to mcts
 * @param root tree to search
 * @param optimizer player to find best move for
 * @param budget how many iterations can be taken
 * @return best move for optimizer
 */
uint64_t monte_carlo_tree_search(Node *root, int optimizer, int budget);