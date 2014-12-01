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

cardinalDirection Graph_Utils::parse_direction(float heading) {
    cardinalDirection return_dir = 0;

    // Divide heading (0-360f) into 24 segements of 15 degrees each
    switch ((int) floor(heading / 15)) {
        case 0:
            return_dir = N;
            break;
        case 1:
            return_dir = NE;
            break;
        case 2:
            return_dir = NE;
            break;
        case 3:
            return_dir = NE;
            break;
        case 4:
            return_dir = NE;
            break;
        case 5:
            return_dir = E;
            break;
        case 6:
            return_dir = E;
            break;
        case 7:
            return_dir = SE;
            break;
        case 8:
            return_dir = SE;
            break;
        case 9:
            return_dir = SE;
            break;
        case 10:
            return_dir = SE;
            break;
        case 11:
            return_dir = S;
            break;
        case 12:
            return_dir = SW;
            break;
        case 13:
            return_dir = SW;
            break;
        case 14:
            return_dir = SW;
            break;
        case 15:
            return_dir = SW;
            break;
        case 16:
            return_dir = SW;
            break;
        case 17:
            return_dir = W;
            break;
        case 18:
            return_dir = W;
            break;
        case 19:
            return_dir = NW;
            break;
        case 20:
            return_dir = NW;
            break;
        case 21:
            return_dir = NW;
            break;
        case 22:
            return_dir = NW;
            break;
        case 23:
            return_dir = N;
            break;
        default:
            return_dir = 0;
    }

    return return_dir;
}

int Graph_Utils::confirmDirection(nodeLabel node1, std::vector<cardinalDirection > openings, Sennot_Graph graph) {
    bool confirmation = true;

    Node * to_check = graph.get_node(node1);

    int direction_check[4] = {0};

    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        // If we are supposed to have a neighbor but parsed information does not reflect this
        // we set the result to false
        if ( to_check->neighbors[i].second != -1 && openings[i] == -1) {
            confirmation = false;
        }
    }

    return confirmation;
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
