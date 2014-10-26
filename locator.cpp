#include "locator.h"

/**
*  \return retb true/false whether the locator has reduced set of traversals to one
*/
bool Locator::is_located() {
    bool retb = false;

    if (num_paths == 0) {
        // We have lost our position
        exit(1);
    }

    // If all possible paths reduced to 1, then we have found our location
    if (num_paths == 1) {
        retb = true;
    }

    return retb;
}

/**
* Prunes possible paths through graph to correspond with how many lights the current path has passed.
*
*
* \param path_cost cost of the edge we are currently travelling upon
*/
int Locator::graph_step(int edge_cost) {

    int keep_path = 0;

    for (int i = 0; i < NODE_COUNT; i++) {
        if (step_lists[i].size() > depth) {

            Node *temp = step_lists[i].back();

            //printf("We are at %c \n",temp->node_label);

            for (int j = 0; j < MAX_NEIGHBORS; j++) {
                // If at least one of the neighbors of the node represents a valid movement we can keep this traversal
                // in set of possible routes
                if (temp->neighbors[j].second != -1 && temp->neighbors[j].second > edge_cost) {
                    //
                    keep_path = 1;
                }
            }

            // If all paths are less than edge progress, then we remove the base from vector
            if (!keep_path) {
                step_lists[i].clear();
                num_paths--;
            }
        }
    }

    return 0;
}


/**
*  \param step_count number of steps that were travelled along edge to reach this node (intersection)
*/
int Locator::graph_intersect(int step_count) {


    for (int i = 0; i < NODE_COUNT; i++) {
        if (step_lists[i].size() > depth) {
            Node *temp = step_lists[i].back();

            // int next_dir = next_step(recent_metrics);
            int next_dir = next_step_m();
            // Iterate through all options NOT in the direction we came from
            for (int j = 0; j < MAX_NEIGHBORS; j++) {
                if (temp->neighbors[j].second != -1 && j == next_dir) {
                    // Check if we have an opening

                    // Turn in this direction
                    depth++;
                    step_lists[i].push_back(temp->neighbors[j].first);
                    return convert_dir(next_dir, curr_heading);


                }
            }
        }
    }
    // reset progress
    edge_progress = 0;

    return -1;
}

/**
* \param heading value in float of heading in 360 degrees
Converts heading to int
#define N 1
#define NE 2
#define E 3
#define SE 4
#define S 5
#define SW 6
#define W 7
#define NW 8
*/
int parse_direction(float heading) {
    int return_dir = 0;

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

/**
*
* Checks openings based on sensor data and places the results in directions vector
*
* \param packet recent metrics from sensors
* \param directions length4 vector
* \param curr_direction direction (cardinal) that the user is currently facing
*/
int Locator::check_openings(Arduino_packet &packet, std::vector<int> &directions, int curr_direction) {
    int retV = 0;

    if (packet.l_distance < INTERSECTION_THRESHOLD) {
        directions[(3 + curr_direction) % 4] = -1;
        retV = 1;
    }

    if (packet.r_distance < INTERSECTION_THRESHOLD) {
        directions[(1 + curr_direction) % 4] = -1;
        retV = 1;
    }

    if (packet.front_distance < INTERSECTION_THRESHOLD) {
        directions[curr_direction] = -1;
        retV = 1;
    }

    // Back is is open since that is where we came from
    directions[(2 + curr_direction) % 4] = 1;


    return retV;
}

/**
*  Allows conversion between world N,S,W,E and L,R,F,B for user
*
*  \param dir to turn relative to heading
*  \param heding we are facing
*/
int Locator::convert_dir(int dir, int heading) {
    int new_dir;

    new_dir = (heading + dir) % 4;

    return new_dir;
}


int Locator::next_step_m() {

    std::vector<int> directions = {N, E, S, W};
    int origin;
    int l;
    int r;
    int f;

    std::cout << "What direction are you facing?" << std::endl;
    std::cin >> origin;
    curr_heading = origin;

    std::cout << "Is left open 0/1?" << std::endl;
    std::cin >> l;

    std::cout << "Is forward open 0/1?" << std::endl;
    std::cin >> f;

    std::cout << "Is r open 0/1?" << std::endl;
    std::cin >> r;

    if (!l) {
        directions[(3 + origin) % 4] = -1;
    }

    if (!r) {
        directions[(1 + origin) % 4] = -1;
    }

    if (!f) {
        directions[origin] = -1;
    }

    int opposite_dir = (origin + 2) % 4;
    // We can not decide to direct our turn the way we came
    directions[opposite_dir] = -1;

    for (int i = 0; i < directions.size(); i++) {
        if (directions[i] != -1) {
            return directions[i];
        }
    }

    return -1;
}

/**
Based on possible possible locations decide which direction to have user turn

@packet most recent metrics acquired from analog sensors

returns direction to turn (value from 0 to 3), -1 if no valid turns found
this indicates that locator is in an unknown state and re-setting may be necessary.
*/
int Locator::next_step(Arduino_packet &packet) {
    int direction = parse_direction(packet.heading);

    std::vector<int> directions = {N, E, S, W};

    // remove directions that are not open
    check_openings(packet, directions, direction / 6);

    int opposite_dir = (direction + 4) % 8;
    // We can not decide to direct our turn the way we came
    directions[opposite_dir - 1] = -1;

    for (int i = 0; i < directions.size(); i++) {
        // -1 is sentinel for invalid direction
        if (directions[i] != -1) {
            return directions[i];
        }
    }

    // In case
    return -1;
}

void Locator::run_locator() {

    int intersection = 0;

    int step;
        old_res = res;
    res = proc.step_detect(this->camera, intersection);
        step = (res ^ old_res) & res;
        if (step) {
            std::cout << "Detected Light" << std::endl;
            ///
            step = graph_step(edge_progress++);
            if (intersection) {

                // Prompt user that he has reached intersection
                Audio::intersection();

                // We are at intersection, check to see which paths we could possibly be on
                int dir = Locator::graph_intersect(edge_progress);
                Audio::turn_dir(dir);
            }
            if (step == 1) {

            }
        }
}


//int Locator::start(std::string serial_info, std::string receive_data) {
//    int serial_id;
//    int retv;
//    // Initialize the serial read
//
//    serial_id = open(serial_info, O_RDONLY | O_NOCTTY | O_NDELAY );
//
//
//
//
//    if (serial_id != -1) {
//        Locator::thread_halt = FALSE;
//        arduino_connection(receive_data,serial_info);
//
//        retv = TRUE;
//    } else { // Serial port was not opened successfully
//        retv = FALSE;
//    }
//
//    return retv;
//}

//void Locator::receive_data(int serial_id) {
//    Arduino_packet read = {0};
//
//    while (!thread_halt) {
//        // Treat read as a buffer and read values to it.
//
//        int byte_count = read(serial_id, &read, sizeof(float) * 5);
//        std::cout << "Read % bytes into the Arduino_packet" << byte_count;
//
//        if (byte_count == sizeof(Arduino_packet)) {
//            this->recent_metrics.back_distance = read.back_distance;
//            this->recent_metrics.front_distance = read.front_distance;
//            this->recent_metrics.heading = read.heading;
//            this->recent_metrics.l_distance = read.l_distance;
//            this->recent_metrics.r_distance = read.r_distance;
//        }
//
//        //Sleep for 100 ms before reading next
//        std::chrono::milliseconds dura( 100 );
//        std::this_thread::sleep_for( dura );
//
//    }
//
//    close(serial_id);
//}


Locator::Locator(std::string file_uri, std::string serial_id) {
    std::ofstream myfile;

    this->proc = Image_Processor();
    this->camera = Camera_Connector(USB_WEBCAM, "", DEFAULT_CAMERA);
    this->edge_progress = 0;
    this->res = 0;
    this->old_res = 1;
//  this->step_lists.resize(NODE_COUNT);

    char nodes[NODE_COUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};
    char edges[12][4] = {{'A', 'B', 3, 0}, {'B', 'C', 3, 0}, {'C', 'D', 4, 0}, {'A', 'E', 2, 0}, {'E', 'F', 1, 0}, {'F', 'G', 2, 0}, {'G', 'H', 4, 0}, {'C', 'H', 5, 0}, {'D', 'J', 5, 0}, {'J', 'I', 0, 0}, {'I', 'L', 0, 0}, {'L', 'K', 0, 0}};
    depth = 0;
    num_paths = NODE_COUNT;
    for (int i = 0; i < NODE_COUNT; i++) {
        // set initial steps
        (this->graph[i]) = new Node(nodes[i]);
        step_lists.at(i).push_back(this->graph[i]);
    }

    // For each each in graph, add connection from node0 to node1 in one direction, then add edge from node1 to node0 in the opposite
    // direction
    for (int i = 0; i < 12; i++) {
        (this->graph.at(edges[i][0] - 65))->add_neighbor(graph.at(edges[i][1] - CHAR_TO_POSITION), edges[i][2], edges[i][3]);
        (this->graph.at(edges[i][1] - 65))->add_neighbor(graph.at(edges[i][0] - CHAR_TO_POSITION), edges[i][2], (edges[i][3] + 2) % 4);
    }

}

Locator::~Locator() {
    thread_halt = TRUE;
    for (int i = 0; i < NODE_COUNT; i++) {
        delete(this->graph[i]);
    }

//    if (arduino_connection.joinable()) {
//        arduino_connection.join();
//    }

}