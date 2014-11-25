#ifndef NODE_H
#define NODE_H

#define MAX_NEIGHBORS 4
#define INVALID_NEIGHBOR -1

typedef char nodeLabel;

class Node {
private:
    int neighbor_count;
public:
    std::pair<Node *, int> * neighbors;
    nodeLabel node_id;

    // ID is the label of this node, neighbor ID is that of the neighbor, cost is num lights
    // to reach neighbor, direction is 0-3 (N S E W) that the neighbor is in
    Node(nodeLabel id);

    Node();

    ~Node();

    void add_neighbor(Node *neighbor_id, int cost, int direction);

    int num_neighbors();

    bool get_neighbor(Node &neighbor_ret, int direction);

    /**
    * Checks whether neighbor in the specified direction
    * is valid.
    */
    bool neighbor_valid(int direction);

};


#endif