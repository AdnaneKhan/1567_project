#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "Sennot_Graph.h"
#include "Arduino_Connector.hpp"
#include "Locator.h"

#include "Node.h"



#define N 1
#define NE 2
#define E 3
#define SE 4
#define S 5
#define SW 6
#define W 7
#define NW 8

#define LONG_DISTANCE 100
namespace Graph_Utils {
    /*
        Checks the connection between two nodes.

        Returns the direction of the connection (from node1 to node2) if it exists

        returns
     */
    cardinalDirection check_connection(nodeLabel node1, nodeLabel node2, Sennot_Graph graph);

    /*
        Checks the node and a packet, checks whether data from the packet aligns with characteristics of a node.

        For each opening registered by graph data, do openings from sonars (in the L, R, and F directions)
        correspond with nodes?

     */
    int confirmDirection(nodeLabel node1, std::vector<cardinalDirection > openings, Sennot_Graph graph);

    cardinalDirection parse_direction(float heading);

    /**
    *  Allows conversion between world N,S,W,E and L,R,F,B for user
    *
    *  \param dir to turn relative to heading
    *  \param current_heading we are facing
    */
    int convert_dir(int to_convert, int current_heading);

}

#endif