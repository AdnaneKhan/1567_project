#include "Locator.h"

locatorState Locator::reset_state() {
    // Reset Graph State

    Audio::play_reset();
    // Play reset information for user FULL_STOP!!
    this->goal_progression = 0;
    this->init_intersect = 0;
    this->goal_list.clear();

    locator_graph.~Sennot_Graph();

    locator_graph = Sennot_Graph();

    return 1;
}

/**
*  \return retb true/false whether the locator has reduced set of traversals to one
*/
bool Locator::is_located() {
    bool retb = false;

    int path_num = this->locator_graph.path_count();

    // If all possible paths reduced to 1, then we have found our location
    if (path_num == 1) {
     //   retb = true;
    }

    return retb;
}

/**
*
* Checks openings based on sensor data and places the results in directions vector
*
* \param packet recent metrics from sensors
* \param directions length4 vector
* \param curr_direction direction (cardinal) that the user is currently facing
*
* \return whether any F,L,R values were detected as open
*/
std::vector<cardinalDirection> Locator::check_openings(Arduino_Packet &packet, cardinalDirection curr_direction) {
    std::vector<cardinalDirection> directions = {DIR_N, DIR_E, DIR_S, DIR_W};
    int l;
    int r;
    int f;
    // Reverse is obviously open
    int b = 1;

    if (this->left_distance  < INTERSECTION_THRESHOLD) {
        directions[LEFT] = -1;
    }

    if (this->right_distance < INTERSECTION_THRESHOLD) {
        directions[RIGHT] = -1;
    }

    if (this->forward_distance < INTERSECTION_THRESHOLD) {
        directions[FORWARD] = -1;
    }


    return directions;
}

std::vector<cardinalDirection> Locator::check_open_m(){

    std::vector<cardinalDirection> directions = {DIR_N, DIR_E, DIR_S, DIR_W};
    int origin;
    int l;
    int r;
    int f;

    int b = 1;


    std::cout << "Is left open 0/1?" << std::endl;
    std::cin >> l;

    std::cout << "Is forward open 0/1?" << std::endl;
    std::cin >> f;

    std::cout << "Is r open 0/1?" << std::endl;
    std::cin >> r;

    if (!l) {
        directions[LEFT] = -1;
    }

    if (!r) {
        directions[RIGHT] = -1;
    }

    if (!f) {
        directions[FORWARD] = -1;
    }

    return directions;
}

cardinalDirection Locator::next_step(std::vector<cardinalDirection> &directions) {


    if (directions[FORWARD] != INVALID_NEIGHBOR) {
        return FORWARD;
    }

    if (directions[RIGHT] != INVALID_NEIGHBOR) {
        return RIGHT;
    }

    if (directions[LEFT] != INVALID_NEIGHBOR) {
        return LEFT;
    }

    if (directions[BACK] != INVALID_NEIGHBOR) {
        return BACK;
    }

    return 0;
}

detectionResult Locator::intersection_check() {
    detectionResult retV = 0;

    std::cout << this->right_distance<< std::endl;
    std::cout << this->left_distance << std::endl;

    // If left or right side shows distances corresponding with an intersection
    if (this->right_distance > INTERSECTION_THRESHOLD || this->left_distance > INTERSECTION_THRESHOLD) {
        retV = INTERSECTION;
    }

    if (this->forward_distance < FRONT_TRESHOLD) {
        retV = INTERSECTION;
    }

    return retV;

};

void Locator::read_distances() {
    this->recent_metrics.start_read();

    this->forward_distance = recent_metrics.read(FRONT_DISTANCE);
    this->left_distance = recent_metrics.read(LEFT_DISTANCE);
    this->right_distance= recent_metrics.read(RIGHT_DISTANCE);

    this->recent_metrics.end_read();
}

void Locator::run_locator() {

    this->read_distances();

    // Rising edge for ceiling light detection
    old_res = res;
    old_intersection = intersection;

    res = proc.step_detect(this->camera, intersection);

    locatorState new_light = (res ^ old_res) & res;

    // Need to ensure we don't count circles as lights too
    if (intersection) {
        new_light = 0;
    }

    // If intersection detected from hallway opening
    intersection |= intersection_check();


    // Check to make sure we don't double count intersection that we are standing under
    // If accelerometers can be incorporated into this that data can also be used
    // to verify.
    intersection_verify(intersection, old_intersection);


    // Indicate to user that we have passed under a light.
    if (new_light) {
        Audio::play_light();
        /// Trims paths based on edge_weights
        locator_graph.edge_step();
    }
}

void Locator::intersection_verify(detectionResult intersect, detectionResult old_intersect) {
    if ((intersect ^ old_intersect) & intersect) {

        // Prompt user that he has reached intersection
        Audio::intersection();

        cardinalDirection to_turn;

        if (locator_graph.path_count() == 1 && !goal_progression) {
            goal_setup();
        }

        if (goal_progression) {
            to_turn = goalDirection();

            handDirection turn_prompt = Graph_Utils::cardinal_to_hand(to_turn, curr_heading);
            std::cout << "Returning " << to_turn;
            std::cout << "Curr" << curr_heading;

            curr_heading = to_turn;

            Audio::turn_dir(turn_prompt);

        } else {
            to_turn = standardDirection(to_turn);

            if (locator_graph.path_count() == 1 && !goal_progression) {
                goal_setup();
                to_turn = goalDirection();
                handDirection turn_prompt = Graph_Utils::cardinal_to_hand(to_turn, curr_heading);
                curr_heading = to_turn;

                Audio::turn_dir(turn_prompt);
            } else {
                Audio::turn_dir(to_turn);
            }
        }
    }
}

// Tasks to carry out once location has been determined
// Fires off pathfinding to goal node and
// changes further commands to guide user via shortest path
// to goal.
void Locator::goal_setup() {
    nodeLabel parent;
    last_node = locator_graph.get_last_node(locator_graph.get_depth(),parent);

    if(last_node == INVALID_NEIGHBOR) {
        this->reset_state();

    } else {
        this->curr_heading = Graph_Utils::check_connection(parent, last_node, this->locator_graph);

        // Identify location direction

        if (!goal_progression) {
            Audio::play_goal();
            goal_list = locator_graph.find_path(last_node, GOAL_NODE);

            goal_progression = TRUE;
        }
    }
}

cardinalDirection Locator::standardDirection(cardinalDirection to_turn) {
    std::vector<cardinalDirection> openings = check_openings(this->recent_metrics, curr_heading);

    to_turn = this->next_step(openings);

 //  locator_graph.intersection_update(to_turn,openings);
   locator_graph.intersection_update(openings);

    return to_turn;
}

cardinalDirection Locator::goalDirection() {
    cardinalDirection to_turn;
    if (goal_list.size() == 1) {
                Audio::play_destination();
            } else {
                // Check connection between current and front of goal list
                to_turn = Graph_Utils::check_connection( goal_list.front(), goal_list.at(1), locator_graph);

                goal_list.erase(goal_list.begin());
            }
    return to_turn;
}

int Locator::start(std::string data_source, int source_type) {
    int retv= 0;

    if (source_type == ARDUINO_DATA) {
        // Initialize the serial read
        con = new Arduino_Connector(&this->recent_metrics, data_source, ARDUINO);

        con->init_connection();
        con->start_thread();
        retv = 1;

    } else if (source_type == SIMULATED_DATA) {
        con = new Arduino_Connector(&this->recent_metrics, data_source, SIMULATION);
        con->start_thread();
        retv =  1;
    }

    Audio::play_start();

    return retv;
}

Locator::Locator(std::string file_uri, int run_type) {

    this->proc = Image_Processor();

    if (run_type == ARDUINO) {
        #ifdef __arm__
        this->camera = Camera_Connector(Camera_Type::RASPBERRY_PI_CAM_E, file_uri, 0);
        #else
        this->camera = Camera_Connector(Camera_Type::USB_WEBCAMS_E , file_uri, DEFAULT_CAMERA);
        #endif

    } else if (run_type == SIMULATION) {
        this->camera = Camera_Connector(Camera_Type::IMAGE_FOLDER_E, file_uri, 180);
    }

    this->res = 0;
    this->old_res = 1;
    this->intersection = 0;
    this->old_intersection = 1;
    this->init_intersect = 0;
}

Locator::~Locator() {

    int end = 0;
    do {
        end = con->stop_thread();
    } while (!end);
}