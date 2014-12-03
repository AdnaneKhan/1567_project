#include "Locator.h"

locatorState Locator::reset_state() {
    // Reset Graph State


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

    if (packet.read(LEFT_DISTANCE)  < INTERSECTION_THRESHOLD) {
        directions[(3 + curr_direction) % 4] = -1;
        l = 1;
    }

    if (packet.read(RIGHT_DISTANCE) < INTERSECTION_THRESHOLD) {
        directions[(1 + curr_direction) % 4] = -1;
        r = 1;
    }

    if (packet.read(FRONT_DISTANCE) < INTERSECTION_THRESHOLD) {
        directions[curr_direction] = -1;
        f = 1;
    }

    if (!l) {
        directions[Graph_Utils::hand_to_cardinal(LEFT, curr_heading)] = -1;
    }

    if (!r) {
        directions[Graph_Utils::hand_to_cardinal(RIGHT, curr_heading)] = -1;
    }

    if (!f) {
        directions[Graph_Utils::hand_to_cardinal(FORWARD, curr_heading)] = -1;
    }

    return directions;
}

std::vector<cardinalDirection> Locator::check_open_m(){

    std::vector<cardinalDirection> directions = {DIR_N, DIR_E, DIR_S, DIR_W};
    int origin;
    int l;
    int r;
    int f;
    // Reverse is obviously open
    int b = 1;

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
        directions[Graph_Utils::hand_to_cardinal(LEFT, curr_heading)] = -1;
    }

    if (!r) {
        directions[Graph_Utils::hand_to_cardinal(RIGHT, curr_heading)] = -1;
    }

    if (!f) {
        directions[Graph_Utils::hand_to_cardinal(FORWARD, curr_heading)] = -1;
    }

    return directions;
}

cardinalDirection Locator::next_step_m(std::vector<cardinalDirection > & directions) {

    if (directions[Graph_Utils::hand_to_cardinal(FORWARD, curr_heading)] != -1) {
        return directions[Graph_Utils::hand_to_cardinal(FORWARD, curr_heading)];
    }


    if (directions[Graph_Utils::hand_to_cardinal(LEFT, curr_heading)] != -1 ) {
        return directions[Graph_Utils::hand_to_cardinal(LEFT, curr_heading)];
    }

    if (directions[Graph_Utils::hand_to_cardinal(RIGHT, curr_heading)] != -1) {
        return directions[Graph_Utils::hand_to_cardinal(RIGHT, curr_heading)];
    }


    if (directions[Graph_Utils::hand_to_cardinal(BACK, curr_heading)] != -1) {
        return directions[Graph_Utils::hand_to_cardinal(BACK, curr_heading)];
    }

    return 0;
}

/**
Based on possible possible locations decide which direction to have user turn

@packet most recent metrics acquired from analog sensors

returns direction to turn (value from 0 to 3), -1 if no valid turns found
this indicates that locator is in an unknown state and re-setting may be necessary.
*/
cardinalDirection Locator::next_step(Arduino_Packet &packet) {
    cardinalDirection to_turn = Graph_Utils::parse_direction(packet.read(TURNING));

    std::vector<handDirection> directions = {FORWARD, RIGHT, BACK, LEFT};

    // remove directions that are not open
    check_openings(packet, to_turn / 6);

    cardinalDirection opposite_dir = (to_turn + 4) % 8;
    // We can not decide to direct our turn the way we came
    directions[opposite_dir - 1] = INVALID_DIRECTION;

    for (cardinalDirection candidate : directions) {
        if (candidate != INVALID_DIRECTION) {
            return candidate;
        }
    }

    // In case no directions found that are open
    return INVALID_DIRECTION;
}

int Locator::intersection_check(Arduino_Packet & check) {
    int retV = 0;


    if((check.read(TURNING) != 0) && ((check.read(FRONT_DISTANCE) < FRONT_TRESHOLD) || (check.read(RIGHT_DISTANCE) > INTERSECTION_THRESHOLD) || check.read(LEFT_DISTANCE) > INTERSECTION_THRESHOLD)) {
        retV = 1;
    }

//    int d_e;
//    std::cout << "Are you at a intersection 1/0?" << std::endl;
//    std::cin >> d_e;
//
//    if (d_e) {
//        retV = 1;
//    }

    // Check if there are op
    //enings in any direction but the front
    // that exceed the detection thresholds

    // if theere are then mark this as an intersection

    // note that I will likely need to implement some sort of voting
    // procedure where values must occur twice in a row in order to be confirmed as an intersection.

    return retV;

};

void Locator::run_locator() {

    std::cout << "Front: " <<this->recent_metrics.read(FRONT_DISTANCE) << std::endl;
    std::cout << "Right: " <<this->recent_metrics.read(RIGHT_DISTANCE)<< std::endl;
    std::cout << "Left: " <<this->recent_metrics.read(LEFT_DISTANCE) << std::endl;
    std::cout << "Heading: " <<this->recent_metrics.read(TURNING) << std::endl;

    curr_heading = (Graph_Utils::parse_direction(this->recent_metrics.read(TURNING)) -1)/2;

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
    intersection |= intersection_check(this->recent_metrics);


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
        } else {
            to_turn = standardDirection(to_turn);
        }

        // Update state of graph based on direction that we turned


        cardinalDirection turn_command = Graph_Utils::cardinal_to_hand(to_turn, curr_heading);
#ifdef DEBUG
        std::__1::cout << "The turn was " << to_turn << std::__1::endl;
#endif
        Audio::turn_dir(turn_command);

    }
}

// Tasks to carry out once location has been determined
// Fires off pathfinding to goal node and
// changes further commands to guide user via shortest path
// to goal.
void Locator::goal_setup() {
    last_node = locator_graph.get_last_node(locator_graph.get_depth());


    if (!goal_progression) {
        Audio::play_goal();
        goal_list = locator_graph.find_path(last_node, GOAL_NODE);

        goal_progression = TRUE;
    }
}

cardinalDirection Locator::standardDirection(cardinalDirection to_turn) {
    std::vector<cardinalDirection> openings =  check_openings(this->recent_metrics, curr_heading);
    to_turn = this->next_step_m(openings);
    if (!init_intersect) {
                // First intersection reached
                locator_graph.edge_progress = 0;
                init_intersect = 1;
            }
        locator_graph.intersection_update(to_turn,openings);

    return to_turn;
}

cardinalDirection Locator::goalDirection() {
    cardinalDirection to_turn;
    if (goal_list.size() == 1) {
                Audio::play_destination();
            } else {
                // Check connection between current and front of goal list
                to_turn = Graph_Utils::check_connection( goal_list.front(), goal_list.at(1), locator_graph);
                std::vector<cardinalDirection> temp_v = check_openings(this->recent_metrics, curr_heading);
             //   std::vector<cardinalDirection> temp_v = check_open_m();
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