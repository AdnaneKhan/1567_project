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

#define FORWARD 0
#define RIGHT 1
#define BACK 2
#define LEFT 3

#define DEBUG

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
    typedef struct Arduino_packet {
        float heading;
        float l_distance;
        float r_distance;
        float back_distance;
        float front_distance;

    } Arduino_packet;

    std::array<Node *, 12> graph;
    std::array<std::list<Node *>, 12> step_lists;


    Arduino_packet recent_metrics;
    Camera_Connector camera;
    std::thread arduino_connection;
    Image_Processor proc;

    int thread_halt;
    int edge_progress;
    int depth;
    int num_paths;
    int curr_heading;
    int old_res;
    int res;

    int intersection;
    int old_intersection;

    void receive_data(int serial_id);

    int intersection_check(Arduino_packet & check);
    int next_step(Arduino_packet &packet);

    int next_step_m();


    /**
    *  Parses sensor data to check which directions are open for the user to turn into
    */
    int check_openings(Arduino_packet &packet, std::vector<int> &directions, int curr_direction);

    /**
    *
    */
    int graph_intersect(int step_count);


    /**
    * Makea a step to a new node in the graph
    */
    int graph_step(int edge_progress);

    /**
    *
    */
    static int convert_dir(int dir, int heading);

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
    void reset_state();


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