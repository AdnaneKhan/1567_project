#include <array>
#include "Node.h"

void Node::clear_tree()
{

    for (int i = 0; i < MAX_NEIGHBORS; i++)
    {
        if (this->neighbors[i].second != INVALID_NEIGHBOR)
        {

            this->neighbors[i].first->clear_tree(); // CLear children of child

            delete(this->neighbors[i].first); // Now this child node can be deleted
            this->neighbors[i].second = INVALID_NEIGHBOR; // Set child as invalid

        }
    }
}

bool Node::get_neighbor(Node &neighbor_ret, int direction)
{
    bool retV = false;

    // Check if value is valid
    if (neighbors[direction].second != INVALID_NEIGHBOR)
    {

        neighbor_ret = (*neighbors[direction].first);

        retV = true;
    }

    return retV;
}

void Node::add_neighbor(Node *neighbor, int cost, int direction)
{

    neighbor_count++; // We now have one more neighbor to htis node

    neighbors[direction].first = neighbor; // In specified direction the new neighbor is added
    neighbors[direction].second = cost; // Cost is set accordingly
}

bool Node::add_neighbor(Node *neighbor, int cost)
{
    bool returnV;


    if (neighbor_count < MAX_NEIGHBORS)
    { // neighbor can only be added if less than 4 exist currently
        neighbors[neighbor_count].first = neighbor;
        neighbors[neighbor_count].second = cost;
        neighbor_count++;

        returnV = true;
    }
    else
    {
        returnV = false;
    }

    return returnV;
}

Node::Node(nodeLabel id)
{
    neighbor_count = 0;

    neighbors = new std::pair<Node *, int>[MAX_NEIGHBORS];

    for (int i = 0; i < MAX_NEIGHBORS; i++)
    {
        neighbors[i].second = INVALID_NEIGHBOR;
    }
    node_id = id;
    this->valid = 1;
}

Node::Node()
{
    this->valid = 1;
    neighbor_count = 0;

    neighbors = new std::pair<Node *, int>[MAX_NEIGHBORS];

    for (int i = 0; i < MAX_NEIGHBORS; i++)
    {
        neighbors[i].second = INVALID_NEIGHBOR;
    }
}

Node::~Node()
{
    delete(neighbors);
}

int Node::num_neighbors()
{
    return neighbor_count;
}