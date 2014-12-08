#include <array>
#include "Node.h"

void Node::clear_tree() {

    // Clears
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if (this->neighbors[i].second != INVALID_NEIGHBOR) {

            this->neighbors[i].first->clear_tree();
            delete(this->neighbors[i].first);
            this->neighbors[i].second = INVALID_NEIGHBOR;

        }
    }
}

bool Node::get_neighbor(Node &neighbor_ret, int direction) {
    bool retV = false;

    // Check if value is valid
    if (neighbors[direction].second != INVALID_NEIGHBOR) {
        neighbor_ret = (*neighbors[direction].first);
        retV = true;
    }

    return retV;
}

void Node::add_neighbor(Node *neighbor, int cost, int direction) {
    neighbor_count++;
    neighbors[direction].first = neighbor;
    neighbors[direction].second = cost;
}

bool Node::add_neighbor(Node *neighbor, int cost) {
    bool returnV;


    if (neighbor_count < 4) {
        neighbors[neighbor_count].first = neighbor;
        neighbors[neighbor_count].second = cost;
        neighbor_count++;

        returnV = true;
    } else {
        returnV = false;
    }

    return returnV;
}

Node::Node(nodeLabel id) {
    neighbor_count = 0;
    neighbors = new std::pair<Node *, int>[MAX_NEIGHBORS];
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        neighbors[i].second = INVALID_NEIGHBOR;
    }
    node_id = id;
}

Node::Node() {
    neighbor_count = 0;
    neighbors = new std::pair<Node *, int>[MAX_NEIGHBORS];
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        neighbors[i].second = INVALID_NEIGHBOR;
    }
}

Node::~Node() {
    delete(neighbors);
}

int Node::num_neighbors() {
    return neighbor_count;
}