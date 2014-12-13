#ifndef SENNOT_GRAPH_H
#define SENNOT_GRAPH_H

#include <list>
#include <array>
#include <queue>

#include "Node.h"
#include "ConstantDefs.hpp"

#define NODE_COUNT 19
#define EDGE_COUNT 19
#define CHAR_TO_POSITION 65

#define ALLOWABLE_LIGHT_ERROR 2

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3

#define INVALID_PREV '!'

typedef int graphInt;


// Nodes in the graph by label
const char nodes[NODE_COUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S'};

// Edges in graph
const char edges[EDGE_COUNT][4] = {{'A', 'B', 3, DIR_E}, {'B', 'H', 5, DIR_S}, {'B', 'C', 3, DIR_E}, {'D', 'C', 2, DIR_S}, {'D', 'F', 2, DIR_E}, {'E', 'F', 1, DIR_S}, {'F', 'J', 4, DIR_S}, {'I', 'J', 1, DIR_E}, {'J', 'K', 3, DIR_S},
        {'G', 'H', 4, DIR_E}, {'H', 'R', 3, DIR_E}, {'R', 'K', 3, DIR_E}, {'R', 'S', 2, DIR_S}, {'K', 'L', 2, DIR_E}, {'K', 'Q', 4, DIR_S}, {'P', 'Q', 5, DIR_E}, {'O', 'P', 2, DIR_S}, {'N', 'O', 1, DIR_E}, {'M', 'O', 2, DIR_S}};

class Sennot_Graph
{

public:

    // Constructor and Desturctor
    Sennot_Graph();
    ~Sennot_Graph();

    /*
    Resets the graph state to reflect initial state of the problem
     */
    void reset_graph();

    /*
    Update the graph state to have reached an intsection while seeing the given directions open in terms of (L,R,F,B) directions
     */
    bool intersection_update(std::vector<handDirection> &dirs_open);

    // Adds a node to the graph
    int add_node(Node *parent, int tree_depth, int num_neighbors, int add_cost, std::vector<handDirection> &dirs_open);

    // Makes a step along the current edge
    graphInt edge_step();

    // Returns number of candidate paths
    graphInt path_count();

    // Returns depth of path
    graphInt get_depth();

    // Returns the last node (ONLY IF LOCATED)
    nodeLabel get_last_node(int path_length, nodeLabel &parent);
    nodeLabel get_last_node(Node *root, int deeper, nodeLabel &parent);
    //


    // Gets node from the graph with teh given node label
    Node *get_node(nodeLabel node);

    /**
    * Finds path from start node to destination node
    * Returns as a list of ints which are the indexes of the nodes
    *
    * Uses Djikstras algorithm
    */
    std::vector<nodeLabel> find_path(Node *start, Node *finish);
    std::vector<nodeLabel> find_path(nodeLabel start, nodeLabel finish);




    // Checks whether we can arrive at node possible step from the approach direction and see the following directions open
    // returns true if this possibility is valid according to the graph.
    bool neighbor_match(Node *possible_step, cardinalDirection approach_dir, std::vector<handDirection> &dirs_open);

private:

    void initialize_paths();
    void initialize_graph();

    // Graph representing sennot square floor 5
    std::array<Node *, NODE_COUNT> graph;

    // List of trees that reflect possible paths taken, when the leaf deepest tree has only 1 node then we are located
    std::list<Node *> progression_tree;

    // Num of candidate paths being tracked
    graphInt num_paths;

    // Whether the first intersection has been reached in this problem cycle
    int init_intersect;
    // Progress on current edge
    graphInt edge_progress;

    // Depth (in number of intersections of traversal)
    graphInt depth;


};

#endif