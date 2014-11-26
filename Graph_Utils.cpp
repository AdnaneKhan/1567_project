#include "Graph_Utils.hpp"

cardinalDirection Graph_Utils::check_connection(nodeLabel node1, nodeLabel node2, Sennot_Graph graph) {
    cardinalDirection retV;

    Node * first_node = graph.get_node(node1);
    Node * second_node = graph.get_node(node2);

    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if ( first_node->neighbors->second != -1 &&  (first_node->neighbors[i].first->node_id == node2)) {
            retV = i;
        }
    }

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

int Graph_Utils::convert_dir(int to_convert, int current_heading) {
    int new_dir;

    new_dir = (current_heading + to_convert) % MAX_NEIGHBORS;

    return new_dir;
}
