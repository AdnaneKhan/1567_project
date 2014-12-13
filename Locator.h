#ifndef LOCATOR_H
#define LOCATOR_H

#include <curses.h>
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

#define INTERSECTION_THRESHOLD 70
#define LEFT_INTERSECTION_TRESHOLD 65
#define FRONT_TRESHOLD 60
#define INTERSECTION 1

#define GOAL_NODE 'S'

#define INVALID_DIRECTION -1

#define FORWARD 0
#define RIGHT 1
#define BACK 2
#define LEFT 3

#define DEBUG

typedef int locatorState;

/*
    This class represents the Sennot Square navigation problem.

    Cost should be measured by the number of rectangular lights that seperate the two graph.
    Nodes are intersections of two or more hallways.

 */
class Locator
{

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
    int start(std::string serial_info, int source_type);

    Locator(std::string file_uri, int run_type);

    ~Locator();

private:

    typedef struct Sonar_Distances
    {
        sensorValue left_distance;
        sensorValue right_distance;
        sensorValue forward_distance;
    } Sonar_Distances;

    // Holds most recent metrics from
    // Arduino, if reading more than one value it is
    // necessary to use start read and end read calls to
    // ensure that all data originated from same packet.
    Arduino_Packet newest_metrics;

    ///=======================================
    /// CONNECTORS AND PROCESSORS FOR HARDWARE
    Camera_Connector camera;
    Image_Processor proc;
    Arduino_Connector *con;
    ///////////////===========================

    Sennot_Graph locator_graph;
    nodeLabel last_node;
    std::vector<nodeLabel> goal_list;

    bool goal_progression; // Are we on path to goal?

    // Value is non valid until location has occured, afterwards value
    // is consistent with "sennot cardinals" as long as user makes appropriate
    // turns in hand direction.
    cardinalDirection curr_heading;

    // Variable for tracking light
    detectionResult old_light_res;
    detectionResult light_res;

    // Variable for tracking intersection
    detectionResult intersection;
    detectionResult old_intersection;

    // Holds the sonar distances read in per loop of locator
    Sonar_Distances curr_cycle;

    // Checks the distances reported from the Arduino to determine whether hallway openings suggest
    // The presence of an intersection
    detectionResult intersection_check();

    detectionResult intersection_check_m(); // simulaed intersection check

    cardinalDirection next_step(std::vector<cardinalDirection> &directions);

    ////////////////////////////////////////////////

    /**
    *  Parses sensor data to check which directions are open for the user to turn into
    */
    std::vector<cardinalDirection> check_openings(Sonar_Distances &distances);

    std::vector<cardinalDirection> check_open_m();

    /**
    *  Resets the state of the graph so that user is located at any of ndes matching current
    *  charactersistics
    *
    * Essentially restarts the locator and pathfinding
    */
    locatorState reset_state();

    // Reads distances from arduino packet into sonar struct
    bool verify_single(sensorDistance & dist1, sensorDistance  & dist2);

    // Sets up state for guiding user back to goal
    void goal_setup();

    // Update distances from Arduino packet
    void read_distances();


    // When we are guiding user back to goal this is used to return the next
    // direction (in sennot cardinal direction) to the next node)
    cardinalDirection goalDirection();

    // In pre-localization state returns the next direction to turn in hand direction
    handDirection standardDirection();

    // Verifies intersection by making sure we aren't already in an intersection state
    void intersection_verify(detectionResult intersect, detectionResult old_intersect);

};

#endif