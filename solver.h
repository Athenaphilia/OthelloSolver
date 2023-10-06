#ifndef SOLVER_H
#define SOLVER_H
#include "position.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct Node {
    Game game; // representation of the game state
    struct Node *parent;
    struct Node **children; // pointers to all the child nodes
    uint64_t *legal_moves;  // legal moves respective to children
    int num_legal;          // number of legal moves
    int num_expanded;       // number of nodes that have been expanded
    uint64_t visits;
    double value;
    bool end;
} Node;

/**
 * Finds if the node is a leaf
 * @param node node to check
 * @return if node still has expansions
 */
bool is_leaf(Node *node);

/**
 * Finds the Upper Confidence Bound of a node
 * Uses a constant of 1.5
 * @param node node to find ucb1 for
 * @param UCB_C constant, higher for exploration, lower for exploitation
 * @return ucb1 score
 */
double UCB(Node *node, double UCB_C);

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
 * @param UCB_C constant, higher for exploration, lower for exploitation *
 * @return best child
 */
Node *select_child(Node *node, double UCB_C);

/**
 * Selects the best node in a tree based on function
 * Currently using ucb1 with greedy algorithm
 * @param root root of tree to search
 * @param UCB_C constant, higher for exploration, lower for exploitation
 * @return the best node in the tree
 */
Node *select_best_node(Node *root, double UCB_C);

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
 * Calculates the memory usage of one node
 * Helper function for calculate_tree_memory_usage
 * @param node node to check
 * @return size of the node in bytes
 */
uint64_t calculate_node_memory_usage(const Node *node);

/**
 * Calculates the memory used by the whole tree
 * @param root root of tree to check
 * @return the size of the tree in bytes
 */
uint64_t calculate_tree_memory_usage(const Node *root);

/**
 * Frees a tree recursively
 * Cleans up dangling references
 * @param root root of tree to free
 * @return the size of the tree in bytes
 */
void free_tree(Node *root);

/**
 * Prints the iteration, tree memory in kb, and the nodes in the tree
 * @param root root of tree
 * @param iteration the iteration mcts is on
 */
void print_debug_info(Node *root, int iteration);

/**
 * Finds the best move according to mcts
 * @param root tree to search
 * @param UCB_C constant, higher for exploration, lower for exploitation
 * @param optimizer player to find best move for
 * @param budget how many iterations can be taken
 * @param debug whether to print debug info
 * @return index of best move for optimizer
 */
int monte_carlo_tree_search(Node *root, double UCB_C, int optimizer, int budget, bool debug);

/**
 * Initializes a root ready for mcts
 * @return initialized root
 */
Node *initialize_root(Game game);
#endif