
#ifndef NODE_H
#define NODE_H

#define MAX_NEIGHBORS 4

class Node {
    int neighbor_count;
public:
    std::pair<Node *, int> *neighbors;
    char node_label;


    // ID is the label of this node, neighbor ID is that of the neighbor, cost is num lights
    // to reach neighbor, direction is 0-3 (N S E W) that the neighbor is in
    Node(char id);

    Node();

    ~Node();

    void add_neighbor(Node *neighbor_id, int cost, int direction);

    int num_neighbors();

    bool get_neighbor(Node &neighbor_ret, int direction);

};


#endif