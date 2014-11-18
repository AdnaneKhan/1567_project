#ifndef SENNOT_GRAPH_H
#define SENNOT_GRAPH_H

#include <list>
#include <array>

#include "Node.h"

#define NODE_COUNT 12
#define CHAR_TO_POSITION 65

typedef int graphInt;
typedef int cardinalDirection;

const char nodes[NODE_COUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};
const char edges[12][4] = {{'A', 'B', 3, 2}, {'B', 'C', 3, 2}, {'C', 'D', 4, 4}, {'A', 'E', 2, 3}, {'E', 'F', 1, 2}, {'F', 'G', 2, 3},
        {'G', 'H', 4, 2}, {'C', 'H', 5, 3}, {'D', 'J', 5, 3}, {'J', 'I', 0, 0}, {'I', 'L', 0, 3}, {'L', 'K', 0, 0}};

class Sennot_Graph {
public:
    Sennot_Graph();
    ~Sennot_Graph();

    graphInt graph_intersect(cardinalDirection next_dir);
    graphInt graph_step();
    graphInt path_count();

private:


    void initialize_paths();
    void initialize_graph();


    std::array<Node *, 12> graph;
    std::array<std::list<Node *>, 12> step_lists;


    graphInt edge_progress;

    graphInt depth;
    graphInt num_paths;

    graphInt edge_cost;

};

#endif