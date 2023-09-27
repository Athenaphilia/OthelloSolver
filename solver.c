#include "solver.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void shuffle_moves(uint64_t *legal_moves, int num_legal) {

    for (int i = 0; i < num_legal - 1; i++) {
        int j = i + rand() / (RAND_MAX / (num_legal - i) + 1);
        uint64_t temp = legal_moves[j];
        legal_moves[j] = legal_moves[i];
        legal_moves[i] = temp;
    }
}

Node *select_child(Node *node) {
    return NULL;
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

int expand_node(Node *node, int optimizer) {

    // get the index of the child to expand to
    int child_index = 0;
    while (node->children[child_index] != NULL) {
        child_index++;
    }

    // assign the child it's parameters
    Node *child = (Node *)malloc(sizeof(Node));
    child->game = make_move(node->game, node->legal_moves[child_index]);
    child->parent = node;
    child->visits = 0;
    child->value = 0;
    int state = find_state(child->game);

    // check the state to make sure we are not exploring
    if (state == 1) {
        // child is a terminal state, backpropagate immediately
        // should be refactored to happen in simulate function
        child->end = true;
        backpropogate(child, get_score_from_state(child->game, optimizer));
        return 1;
    } else if (state == 2) {
        // child state passes, change the player manually
        child->game.player = 3 - child->game.player;
    }
    child->end = false;

    // gets the number of legal moves by using a temp array
    // then mallocs the actual array and assigns the temp to the array
    // this is hacky, but I would need to write a new function make it less hacky
    uint64_t temp_legal_moves[MAX_LEGAL_MOVES];
    child->num_legal = generate_legal_moves(child->game, temp_legal_moves);
    child->legal_moves = (uint64_t *)malloc(child->num_legal * sizeof(uint64_t));
    for (int i = 0; i < child->num_legal; i++) {
        child->legal_moves[i] = temp_legal_moves[i];
    }
    // shuffle the moves at creation to avoid duplicate children
    shuffle_moves(child->legal_moves, child->num_legal);

    // assign the child to the node's children
    node->children[child_index] = child;

    // child has more moves to make
    return 0;
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

    // cut the dangling reference
    // this could be skipped for speed, but since it happens at the end of a search, it's
    // more proper to do it this way
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

int main() {
}