#include "solver.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DEBUG_INTERVAL 1000

bool is_leaf(Node *node) {
    return node->num_expanded >= node->num_legal;
}

double UCB(Node *node, double UCB_C) {
    return (node->value / node->visits) + UCB_C * (sqrt(log(node->parent->visits) / node->visits));
}

void shuffle_moves(uint64_t *legal_moves, int num_legal) {

    for (int i = 0; i < num_legal - 1; i++) {
        int j = i + rand() / (RAND_MAX / (num_legal - i) + 1);
        uint64_t temp = legal_moves[j];
        legal_moves[j] = legal_moves[i];
        legal_moves[i] = temp;
    }
}

int random_move(int num_legal) {
    return rand() % num_legal;
}

Node *select_child(Node *node, double UCB_C) {

    Node *best_child = NULL;
    double best_ucb = -INFINITY;
    // iterate through the children and
    for (int i = 0; i < node->num_legal; i++) {
        Node *child = node->children[i];
        if (child == NULL) {
            // No more valid children, break
            break;
        }
        double ucb = UCB(child, UCB_C);
        if (ucb > best_ucb) {
            best_ucb = ucb;
            best_child = child;
        }
    }
    return best_child;
}

Node *select_best_node(Node *root, double UCB_C) {

    while (!root->end) {
        if (!is_leaf(root)) {
            return root;
        } else {
            root = select_child(root, UCB_C);
        }
    }
    return root;
}

double get_score_from_state(Game game, int optimizer) {

    int optimizing_player_pieces = count_pieces(game, optimizer);
    int opponent_player_pieces = count_pieces(game, 3 - optimizer);
    if (optimizing_player_pieces > opponent_player_pieces) {
        return 1.0;
    } else if (optimizing_player_pieces < opponent_player_pieces) {
        return 0.0;
    } else {
        return 0.5;
    }
}

void backpropogate(Node *node, double result) {

    // update the visit count and value of all node in this path
    while (node != NULL) {
        node->visits++;
        node->value += result;
        node = node->parent;
    }
}

Node *expand_node(Node *node) {

    if (node->end) {
        // node that is being expanded is the end, do not expand
        return node;
    }
    // get the index of the child to expand to
    int child_index = node->num_expanded;

    // assign the child it's parameters
    Node *child = (Node *)malloc(sizeof(Node));
    child->game = make_move(node->game, node->legal_moves[child_index]);
    child->parent = node;
    child->visits = 0;
    child->value = 0;
    int state = find_state(child->game);

    // check the state to make sure we are not exploring terminal states
    if (state == 1) {
        // child is a terminal state: do not allocate any more memory
        child->end = true;
        node->children[child_index] = child;
        node->num_expanded++;
        return child;
    } else if (state == 2) {
        // child state passes, change the player manually
        child->game.player = 3 - child->game.player;
    }
    child->end = false;

    // gets the number of legal moves by using a temp array
    // then mallocs the actual array and assigns the temp to the array
    // this is hacky, but I would need to write a new function to make it less hacky
    uint64_t temp_legal_moves[MAX_LEGAL_MOVES];
    child->num_legal = generate_legal_moves(child->game, temp_legal_moves);
    child->legal_moves = (uint64_t *)malloc(child->num_legal * sizeof(uint64_t));
    child->children = (Node **)malloc(child->num_legal * sizeof(Node *));
    for (int i = 0; i < child->num_legal; i++) {
        child->legal_moves[i] = temp_legal_moves[i];
    }
    // shuffle the moves at creation to avoid duplicate children
    shuffle_moves(child->legal_moves, child->num_legal);

    // assign the child to the node's children
    node->children[child_index] = child;
    node->num_expanded++;

    // child has more moves to make
    return child;
}

double simulate(Game game, int optimizer) {
    int state = find_state(game);
    while (state != 1) {
        uint64_t legal_moves[MAX_LEGAL_MOVES];
        int num_legal = generate_legal_moves(game, legal_moves);
        int move = random_move(num_legal);
        game = make_move(game, legal_moves[move]);
        state = find_state(game);
        if (state == 2) {
            // player passes
            game.player = 3 - game.player;
        }
    }
    return get_score_from_state(game, optimizer);
}

uint64_t count_nodes(const Node *root) {
    if (root == NULL) {
        return 0;
    }

    uint64_t count = 1; // Count the current node

    for (int i = 0; i < root->num_legal; i++) {
        count += count_nodes(root->children[i]);
    }

    return count;
}

uint64_t calculate_node_memory_usage(const Node *node) {
    uint64_t size = sizeof(Node);

    // Add memory used by legal_moves array if it exists
    if (node->legal_moves != NULL) {
        size += sizeof(uint64_t) * node->num_legal;
    }

    return size;
}

uint64_t calculate_tree_memory_usage(const Node *root) {
    if (root == NULL) {
        return 0;
    }

    uint64_t bytes = calculate_node_memory_usage(root);
    for (int i = 0; i < root->num_legal; i++) {
        bytes += calculate_tree_memory_usage(root->children[i]);
    }

    return bytes;
}

void free_tree(Node *root) {
    if (root == NULL) {
        // base case
        return;
    }

    // recursively free all the children
    for (int i = 0; i < root->num_legal; i++) {
        free_tree(root->children[i]);
    }

    // free the legal move array
    if (root->legal_moves != NULL) {
        free(root->legal_moves);
    }
    if (root->children != NULL) {
        free(root->children);
    }

    // cut the dangling reference
    // this could be skipped for speed, but since it happens at the end of a search, it's
    // more proper to do it this way
    // this could be done earlier for efficiency
    if (root->parent != NULL) {
        for (int i = 0; i < MAX_LEGAL_MOVES; i++) {
            if (root->parent->children[i] == root) {
                root->parent->children[i] = NULL;
                break;
            }
        }
    }

    // free the current node
    free(root);
}

void print_debug_info(Node *root, int iteration) {
    printf("%d - Kb: %llu, Nodes: %llu\r", iteration, (unsigned long long)calculate_tree_memory_usage(root) / (1024), (unsigned long long)count_nodes(root));
    fflush(stdout);
}

int monte_carlo_tree_search(Node *root, double UCB_C, int optimizer, int budget, bool debug) {

    int best_move = 0;

    // mcts loop
    // one loop is one iteration of mcts
    for (int i = 0; i < budget; i++) {
        if (debug && !(i % DEBUG_INTERVAL)) {
            print_debug_info(root, i);
        }

        // find the node to expand
        Node *search_node = select_best_node(root, UCB_C);

        // expand the node
        search_node = expand_node(search_node);

        // simulate the result and backpropagate the result
        backpropogate(search_node, simulate(search_node->game, optimizer));
    }
    if (debug) {
        print_debug_info(root, budget - 1);
        putchar('\n');
    }

    uint64_t best_visits = 0;
    for (int child = 0; child < root->num_legal; child++) {
        if (root->children[child]->visits > best_visits) {
            best_visits = root->children[child]->visits;
            best_move = child;
        }
    }
    return best_move;
}

Node *initialize_root(Game game) {
    Node *root;
    root = (Node *)malloc(sizeof(Node));
    root->end = false;
    root->parent = NULL;
    root->game = game;
    root->visits = 0;
    root->value = 0;

    // shuffle the root's moves
    // necessary for expand_node to work
    // copied from expand node
    uint64_t temp_legal_moves[MAX_LEGAL_MOVES];
    root->num_legal = generate_legal_moves(root->game, temp_legal_moves);
    root->legal_moves = (uint64_t *)malloc(root->num_legal * sizeof(uint64_t));
    root->children = (Node **)malloc(root->num_legal * sizeof(Node *));
    for (int i = 0; i < root->num_legal; i++) {
        root->legal_moves[i] = temp_legal_moves[i];
    }
    shuffle_moves(root->legal_moves, root->num_legal);
    return root;
}
