#ifndef NODE_H
#define NODE_H

#define MAX_NEIGHBORS 4
#define INVALID_NEIGHBOR -1


typedef char nodeLabel;
typedef float compassData;

class Node {
private:
    int neighbor_count;
    compassData north_data;
    compassData south_data;
    compassData west_data;
    compassData east_data;

public:
    std::pair<Node *, int> * neighbors;
    nodeLabel node_id;

    nodeLabel visitor;

    // ID is the label of this node, neighbor ID is that of the neighbor, cost is num lights
    // to reach neighbor, direction is 0-3 (N S E W) that the neighbor is in
    Node(nodeLabel id);

    Node();

    ~Node();

    void clear_tree();

    void add_neighbor(Node *neighbor, int cost, int direction);

    // Adds neighbor in an undirected manner
    // If no more space returns false
    // If add was successful returns true
    // Ensure that pointer is tracked and properly
    // dealt with in case of returning false to prevent
    // mmeory leaks.
    bool add_neighbor(Node * neighbor, int cost);

    int num_neighbors();

    bool get_neighbor(Node &neighbor_ret, int direction);

    /**
    * Checks whether neighbor in the specified direction
    * is valid.
    */
    bool neighbor_valid(int direction);

};


#endif