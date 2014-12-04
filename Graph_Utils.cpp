#include "Graph_Utils.hpp"

cardinalDirection Graph_Utils::check_connection(nodeLabel node1, nodeLabel node2, Sennot_Graph & graph) {
    cardinalDirection retV = -1;

    Node * first_node = graph.get_node(node1);

    std::cout << "Checking " << first_node->node_id <<std::endl;


    // We iterate through the list of neighbors of the start node, we check which in which direction
    // the second node is (from the first) and return that direction.
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if ( (first_node->neighbors[i].second != INVALID_NEIGHBOR) && (first_node->neighbors[i].first->node_id == node2)) {
            std::cout << " checking " <<first_node->neighbors[i].first->node_id;
            retV = i;
        }
    }
    std::cout << "Checking " << node1 << " and " << node2 << " for connection \n";
    return retV;
}

int Graph_Utils::cardinal_to_hand(int to_convert, int current_heading) {
    int new_dir;

    switch(current_heading){
        case DIR_E:
               if ( to_convert == DIR_N) {
                    new_dir = LEFT;
               } else if (to_convert == DIR_E) {
                    new_dir = FORWARD;
               } else if (to_convert == DIR_S) {
                    new_dir = RIGHT;
               } else if (to_convert == DIR_W) {
                    new_dir = BACK;
               }
            break;
        case DIR_S:
            if ( to_convert == DIR_N) {
                new_dir = BACK;
            } else if (to_convert == DIR_E) {
                new_dir = LEFT;
            } else if (to_convert == DIR_S) {
                new_dir = FORWARD;
            } else if (to_convert == DIR_W) {
                new_dir = RIGHT;
            }
            break;
        case DIR_W:
            if ( to_convert == DIR_N) {
                new_dir = RIGHT;
            } else if (to_convert == DIR_E) {
                new_dir = BACK;
            } else if (to_convert == DIR_S) {
                new_dir = LEFT;
            } else if (to_convert == DIR_W) {
                new_dir = FORWARD;
            }
            break;
        case DIR_N:
            if ( to_convert == DIR_N) {
                new_dir = FORWARD;
            } else if (to_convert == DIR_E) {
                new_dir = RIGHT;
            } else if (to_convert == DIR_S) {
                new_dir = BACK;
            } else if (to_convert == DIR_W) {
                new_dir = LEFT;
            }
            break;
    }

    return new_dir;
}

int Graph_Utils::hand_to_cardinal(int to_convert, int current_heading) {
    int new_dir;

    new_dir = (to_convert + current_heading) % 4;

    return new_dir;
}
