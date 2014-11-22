#ifndef SENNOT_GRAPH_H
#define SENNOT_GRAPH_H

#include <list>
#include <array>

#include "Node.h"

#define NODE_COUNT 18
#define EDGE_COUNT 20
#define CHAR_TO_POSITION 65

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3

typedef int graphInt;
typedef int cardinalDirection;

// Nodes in the graph by label
const char nodes[NODE_COUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M' ,'N','O','P','Q','R'};

// Edges in the graph
// { NODE1, NODE2, EDGE_COST, DIRECTION }
const char edges[EDGE_COUNT][4] = {{'B', 'C', 3, DIR_E}, {'C', 'D', 3, DIR_E}, {'D', 'E', 4, DIR_E}, {'D', 'A', 2, DIR_N}, {'C', 'F', 1, DIR_S}, {'F', 'G', 2, DIR_E},
        {'G', 'H', 4, DIR_S}, {'H', 'I', 5, DIR_E}, {'E', 'N', 5, DIR_S}, {'D', 'I', 0, DIR_S}, {'H', 'K', 0, DIR_S}, {'J', 'K', 0, DIR_E},{'K','L',0,DIR_E},{'L','J',0,DIR_S},{'I','L',0,DIR_S},{'O','P',0,DIR_S},{'M','N',0,DIR_E},{'M','Q',0,DIR_S},{'P','Q',0,DIR_E},{'Q','R',0,DIR_E}};

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