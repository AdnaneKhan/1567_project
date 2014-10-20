#ifndef CAMERA_INC
#include "camera_connector.h"
#define CAMERA_INC
#endif

#include <thread>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <time.h>   // time calls
#include <iostream>
#include <fstream>
#include<memory>
#include <array>


#define TRUE 1
#define FALSE 0
#define CHAR_TO_POSITION 65

/*
    This class represents the Sennot Square navigation problem.
    A text file is specified in the constructor which contains information to initialize
    a graph.

    Formatting for the graph is as follows:

    Node {neighbor1:cost1,neighbor2:cost2,neighbor3:cost3}

    Cost should be measured by the number of rectangular lights that seperate the two nodes.
    Nodes are intersections of two or more hallways.

 */
class Locator {
    typedef struct Arduino_packet {
        float heading;
        float l_distance;
        float r_distance;
        float back_distance;
        float front_distance;

    } Arduino_packet;



    class Node {
    public:
        char node_label;
        std::array<std::pair<Node,int>, 4> neighbors;
        // ID is the label of this node, neighbor ID is that of the neighbor, cost is num lights
        // to reach neighbor, direction is 0-3 (N S E W) that the neighbor is in
        Node(char id): neighbors() {
            node_label = id;
        }
        void add_neighbor(char neighbor_id, int cost, int direction) {
            neighbors.at(direction) = std::make_pair(sennot_graph.nodes.at(neighbor_id - CHAR_TO_POSITION),cost);
        }
        int num_neighbors() {
            return this->neighbors.size();
        }
        bool get_neighbor(Node & neighbor_ret, int direction) {
            // Check if value is valid
            if (neighbors.at(direction)) {
                neighbor_ret = neighbors.at(direction).first;
            }
        }
    };

    class Graph {
    public:
        std::array<Node, 12> nodes;

    };


    Arduino_packet recent_metrics;
    Camera_Connector camera;
    std::thread arduino_connection;
    int thread_halt;


    Graph sennot_graph;
    int edge_progress;
    std::vector< std::list< char > > step_lists;
    Locator::graph_step();

    // This function is threaded
    void receive_data();

public:

    /*
     Opens a serial connection, if it succeeds, start a thread that reads from the port on a given interval.

     If the connection fails, exit without starting the therad.
     */
    int start();
    /*
    Returns TRUE if the locator has joined possible paths and the location has been narrowed down.
    Returns FALSE if the locator has not yet definitively identified the path that has been taken.

     */
    bool is_located();
    Locator(std::string file_uri, std::string serial_id);
    ~Locator();
};