#ifndef SENNOT_GRAPH_H
#define SENNOT_GRAPH_H

#include <list>
#include <array>

#include "Node.h"

#define NODE_COUNT 15
#define EDGE_COUNT 22
#define CHAR_TO_POSITION 65

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3

typedef int graphInt;
typedef int cardinalDirection;

// Nodes in the graph by label
const char nodes[NODE_COUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M' ,'N','O'};

// Edges in the graph
// { NODE1, NODE2, EDGE_COST, DIRECTION }
const char edges[EDGE_COUNT][4] = {{'A', 'B', 3, 2}, {'B', 'C', 3, 2}, {'C', 'D', 4, 4}, {'A', 'E', 2, 3}, {'E', 'F', 1, 2}, {'F', 'G', 2, 3},
        {'G', 'H', 4, 2}, {'C', 'H', 5, 3}, {'D', 'J', 5, 3}, {'J', 'I', 0, 0}, {'I', 'L', 0, 3}, {'L', 'K', 0, 0}};

class Sennot_Graph {
public:
    Sennot_Graph();
    ~Sennot_Graph();

    graphInt graph_intersect(cardinalDirection next_dir);
    graphInt graph_step();
    graphInt path_count();

    /**
    * Finds path from start node to destination node
    * Returns as a list of ints which are the indexes of the nodes
    */
    std::list<graphInt> find_path(Node * start, Node * finish);


private:

    void initialize_paths();
    void initialize_graph();


    std::array<Node *, NODE_COUNT> graph;
    std::array<std::list<Node *>, NODE_COUNT> step_lists;

    // Progress on current edge
    graphInt edge_progress;

    // Depth (in number of intersections of traversal)
    graphInt depth;

    // Num of candidate paths being tracked
    graphInt num_paths;

};

#endif