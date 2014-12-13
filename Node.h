#ifndef NODE_H
#define NODE_H

typedef int cardinalDirection;
typedef int handDirection;

#define MAX_NEIGHBORS 4
#define INVALID_NEIGHBOR -1

typedef char nodeLabel;

class Node
{
private:
    int neighbor_count;

public:

    std::pair<Node *, int> *neighbors;
    nodeLabel node_id;

    nodeLabel visitor;
    nodeLabel valid;

    // ID is the label of this node, neighbor ID is that of the neighbor, cost is num lights
    // to reach neighbor, direction is 0-3 (N S E W) that the neighbor is in
    Node(nodeLabel id);

    Node();
    ~Node();

    // Clears neighbors of the node
    void clear_tree();

    void add_neighbor(Node *neighbor, int cost, cardinalDirection direction);

    // Adds neighbor in an undirected manner
    // If no more space returns false
    // If add was successful returns true
    // Ensure that pointer is tracked and properly
    // dealt with in case of returning false to prevent
    // mmeory leaks.
    bool add_neighbor(Node *neighbor, int cost);

    /*
       Reeturns the number of neighbors this Node has.
     */
    int num_neighbors();

    /*
        Returns a reference to neighbor in given direction if it exists.
     */
    bool get_neighbor(Node &neighbor_ret, int direction);

};


#endif