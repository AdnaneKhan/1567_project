#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "Sennot_Graph.h"
#include "Arduino_Connector.hpp"
#include "Locator.h"

#include "Node.h"


namespace Graph_Utils {
    /*
        Checks the connection between two nodes.

        Returns the direction of the connection (from node1 to node2) if it exists

        returns
     */
    cardinalDirection check_connection(nodeLabel node1, nodeLabel node2, Sennot_Graph & graph);

    /**
    *  Allows conversion between world N,S,W,E and L,R,F,B for user
    *
    *  \param to_convert direction (cardinal) that we would like to go next
    *  \param current_heading we are facing
    */
    int cardinal_to_hand(int to_convert, int current_heading);

    /**
    *     *  Allows conversion users current heading, a direction they turned relative to heading)
    *
    *
    *  \param to_convert direction (hand) that we turned in/would like to turn in
    *  \param current_heading we are facing in cardinal
    *
    *  \returns direction in cardinal that the user needs to turn
    */
    int hand_to_cardinal(int to_cnvert,int current_heading);



}

#endif