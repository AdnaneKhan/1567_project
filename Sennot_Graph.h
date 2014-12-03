#ifndef SENNOT_GRAPH_H
#define SENNOT_GRAPH_H

#include <list>
#include <array>
#include <queue>

#include "Node.h"

#define NODE_COUNT 19
#define EDGE_COUNT 20
#define CHAR_TO_POSITION 65

#define inrange(a,b) ((a)>(b)?(a):(b))

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3

#define INVALID_PREV '!'

typedef int graphInt;
typedef int cardinalDirection;

// Nodes in the graph by label
const char nodes[NODE_COUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M' ,'N','O','P','Q','R','S'};

// Edges in the graph
// { NODE1, NODE2, EDGE_COST, DIRECTION }
//const char edges[EDGE_COUNT][4] = {{'B', 'C', 1, DIR_E}, {'C', 'D', 3, DIR_E}, {'D', 'E', 3, DIR_E},{'E','F', 4,DIR_E},
//        {'A','E',2,DIR_S},{'C','G',2,DIR_S},{'G','H',1,DIR_E},{'D','I',1,DIR_E},{'E','K',5 ,DIR_S},{'H','J',2,DIR_S},
//        {'J','K',4,DIR_E},{'J','L',3,DIR_S},{'K','M',3,DIR_S},{'F','S',7,DIR_S},{'Q','R',1,DIR_E},{'R','S',3, DIR_E},
//        {'R','U',1, DIR_S},{'N','O',1,DIR_E},{'O','P',4,DIR_E},{'P','T',1,DIR_S}};
const char edges[EDGE_COUNT][4] = {{'A','B',3,DIR_E},{'B','H',5,DIR_S},{'B','C',3,DIR_E},{'D','C',2,DIR_S},{'D','F',2,DIR_E},{'E','F',1,DIR_S},{'F','J',4,DIR_S},{'I','J',1,DIR_E},{'J','K',3,DIR_S},
        {'G','H',4, DIR_E},{'H','R',3,DIR_E},{'R','K',3,DIR_E},{'R','S',2,DIR_S},{'R','K',3,DIR_E},{'K','L',1, DIR_E},{'K','Q',7, DIR_S},{'P','Q',5,DIR_E},{'O','P',2,DIR_S},{'N','O',1, DIR_E},{'M','O',2,DIR_S}};

        class Sennot_Graph {
public:
    Sennot_Graph();
    ~Sennot_Graph();

    void intersection_update(cardinalDirection next_dir,std::vector<cardinalDirection> & dirs_open);

    // Makes a step along the current edge
    graphInt edge_step();

    // Returns number of candidate paths
    graphInt path_count();

    // Returns depth of path
    graphInt get_depth();

    // Returns the last node (ONLY IF LOCATED)
    nodeLabel get_last_node(int path_length);


    Node * get_node(nodeLabel node);

    /**
    * Finds path from start node to destination node
    * Returns as a list of ints which are the indexes of the nodes
    *
    * Uses Djikstras algorithm
    */
    std::vector<nodeLabel> find_path(Node * start, Node * finish);
    std::vector<nodeLabel> find_path(nodeLabel start, nodeLabel finish);
    // Progress on current edge
    graphInt edge_progress;
            // Depth (in number of intersections of traversal)
            graphInt depth;

private:

    void initialize_paths();
    void initialize_graph();


    std::array<Node *, NODE_COUNT> graph;
    std::array<std::list<Node *>, NODE_COUNT> step_lists;



    // Num of candidate paths being tracked
    graphInt num_paths;

};

#endif