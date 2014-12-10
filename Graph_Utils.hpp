#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "Sennot_Graph.h"
#include "Arduino_Connector.hpp"
#include "Locator.h"

#include "Node.h"


namespace Graph_Utils
{
    /*
        Checks the connection between two nodes.

        Returns the direction of the connection (from node1 to node2) if the connection exists

     */
    cardinalDirection check_connection(nodeLabel node1, nodeLabel node2, Sennot_Graph &graph);

    /**
    *
    *
    *  \param to_convert direction (cardinal) that we would like to go next
    *  \param current_heading (cardinal) that we are facing
    *
    *   Returns direction user has to go so that face the new_direction
    *
    */
    handDirection cardinal_to_hand(cardinalDirection new_direction, cardinalDirection current_heading);

    /**
    *     *  Allows conversion users current heading, a direction they turned relative to heading
    *
    *  \param to_convert direction (hand) that we turned in/would like to turn in
    *  \param current_heading we are facing in cardinal
    *
    *  \returns direction in cardinal that the user needs to turn
    */
    cardinalDirection hand_to_cardinal(handDirection to_cnvert, cardinalDirection current_heading);

}

#endif