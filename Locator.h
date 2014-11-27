#ifndef LOCATOR_H
#define LOCATOR_H

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
#include "Graph_Utils.hpp"
#include "Arduino_Connector.hpp"
#include "Sennot_Graph.h"

#define ARDUINO_DATA 1
#define SIMULATED_DATA 2


#define TRUE 1
#define FALSE 0

#define DEFAULT_CAMERA 500
#define MAX_NEIGHBORS 4

#define INTERSECTION_THRESHOLD 12

#define GOAL_NODE 'Y'

#define INVALID_DIRECTION -1



#define FORWARD 0
#define RIGHT 1
#define BACK 2
#define LEFT 3

#define DEBUG


typedef int locatorState;
typedef int handDirection;
/*
    This class represents the Sennot Square navigation problem.
    A text file is specified in the constructor which contains information to initialize
    a graph.

    Formatting for the graph is as follows:

    Node {neighbor1:cost1,neighbor2:cost2,neighbor3:cost3}

    Cost should be measured by the number of rectangular lights that seperate the two graph.
    Nodes are intersections of two or more hallways.

 */
class Locator {

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
    int start(std::string serial_info,int source_type);

    /*
    Returns TRUE if the locator has joined possible paths and the location has been narrowed down.
    Returns FALSE if the locator has not yet definitively identified the path that has been taken.

     */
    bool is_located();

    Locator(std::string file_uri, int run_type);

    ~Locator();
private:

    typedef struct Camera_Readings {
        std::list<detectionResult> result_cache;
        
    } Camera_Readings;

    Arduino_Packet recent_metrics;

    Camera_Connector camera;

    Image_Processor proc;
    Arduino_Connector * con;
    Sennot_Graph locator_graph;

    bool goal_progression;
    std::list<nodeLabel> goal_list;
    nodeLabel last_node;

    cardinalDirection curr_heading;


    int old_res;
    int res;

    detectionResult intersection;
    detectionResult old_intersection;


    int intersection_check(Arduino_Packet & check);
    cardinalDirection next_step(Arduino_Packet &packet);

    int next_step_m();

    /**
    *  Parses sensor data to check which directions are open for the user to turn into
    */
    graphInt check_openings(Arduino_Packet &packet, std::vector<int> &directions, int curr_direction);



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


};

#endif