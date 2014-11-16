#ifndef LOCATOR_H
#define LOCATOR_H

#include <thread>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <time.h>   // time calls
#include <iostream>
#include <fstream>
#include <memory>
#include <array>
#include <stack>
#include <iterator>
#include <list>
#include <cmath>


#include "Camera_Connector.h"
#include "Image_Processor.h"
#include "Node.h"
#include "Audio.h"
#include "Arduino_Connector.hpp"

#define TRUE 1
#define FALSE 0
#define CHAR_TO_POSITION 65
#define DEFAULT_CAMERA 500
#define MAX_NEIGHBORS 4
#define NODE_COUNT 12
#define INTERSECTION_THRESHOLD 12

#define N 1
#define NE 2
#define E 3
#define SE 4
#define S 5
#define SW 6
#define W 7
#define NW 8

#define INVALID_DIRECTION -1

#define FORWARD 0
#define RIGHT 1
#define BACK 2
#define LEFT 3

#define ARDUINO_BUF_SIZE 200

#define DEBUG


typedef int direction;
typedef float sensorDistance;
typedef int graphInt;

typedef int locatorState;
/*
    This class represents the Sennot Square navigation problem.
    A text file is specified in the constructor which contains information to initialize
    a graph.

    Formatting for the graph is as follows:

    Node {neighbor1:cost1,neighbor2:cost2,neighbor3:cost3}

    Cost should be measured by the number of rectangular lights that seperate the two graph.
    Nodes are intersections of two or more hallways.

 */

// Definitions for the sennot square graph
const char nodes[NODE_COUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};
const char edges[12][4] = {{'A', 'B', 3, 2}, {'B', 'C', 3, 2}, {'C', 'D', 4, 4}, {'A', 'E', 2, 3}, {'E', 'F', 1, 2}, {'F', 'G', 2, 3},
        {'G', 'H', 4, 2}, {'C', 'H', 5, 3}, {'D', 'J', 5, 3}, {'J', 'I', 0, 0}, {'I', 'L', 0, 3}, {'L', 'K', 0, 0}};

class Locator {

    std::array<Node *, 12> graph;
    std::array<std::list<Node *>, 12> step_lists;

    Arduino_Packet recent_metrics;

    Camera_Connector camera;


    Image_Processor proc;
    Arduino_Connector * con;

    graphInt edge_progress;

    graphInt depth;
    graphInt num_paths;


    direction curr_heading;


    int old_res;
    int res;

    detectionResult intersection;
    detectionResult old_intersection;


    int intersection_check(Arduino_Packet & check);
    direction next_step(Arduino_Packet &packet);

    int next_step_m();


    /**
    *  Parses sensor data to check which directions are open for the user to turn into
    */
    graphInt check_openings(Arduino_Packet &packet, std::vector<int> &directions, int curr_direction);

    /**
    *
    */
    graphInt graph_intersect(int step_count);


    /**
    * Makea a step to a new node in the graph
    */
    graphInt graph_step(int edge_progress);

    /**
    *  Allows conversion between world N,S,W,E and L,R,F,B for user
    *
    *  \param dir to turn relative to heading
    *  \param current_heading we are facing
    */
    static direction convert_dir(direction to_convert, int current_heading);

//    void parse_packet(char * string_in,int buf_max, Arduino_Packet & to_update);


    /**
    *  Finds a path to the solution from the given node, returns as a list of pair <Node, int> where int is
    *  a parameter to turn direction
    */
    void find_goal();

    /**
    * Guides user to walk until goal hallway open, then
    */
    void goal_walk();


    /**
    *  Resets the state of the graph so that user is located at any of ndes matching current
    *  charactersistics
    */
    locatorState reset_state();

    void initialize_graph();
    void initialize_paths();
public:

    /**
    * Runs the location program one cycle this can either be set to run as a thread or called repeatedly from a main
    * loop
    */
    void run_locator();

    /**
    Opens a serial connection, if it succeeds, start a thread that reads from the port on a given interval.

    If the connection fails, exit without starting the therad.
    */
    int start(std::string serial_info);

    /*
    Returns TRUE if the locator has joined possible paths and the location has been narrowed down.
    Returns FALSE if the locator has not yet definitively identified the path that has been taken.

     */
    bool is_located();

    Locator(std::string file_uri, std::string serial_id);

    ~Locator();
};

#endif