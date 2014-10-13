#ifndef CAMERA_INC
#include "camera_connector.h"
#define CAMERA_INC
#endif

#include <thread>
#include <stdlib>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <time.h>   // time calls
#include <iostream>
#include <fstream>

#define TRUE 1
#define FALSE 0

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

    typedef struct Node {
        int visited;

        int num_neighbors;
        //  (num_neighbors-1) serves as max index into neighbors
        Node neighbors[4];
    } Node;


    Arduino_packet recent_metrics;
    Camera_Connector camera;
    std::thread arduino_connection;
    int thread_halt;

    // Array initalized once graph file is read in
    Node graph[];

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