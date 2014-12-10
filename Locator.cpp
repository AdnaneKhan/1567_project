#include "Locator.h"

locatorState Locator::reset_state() {
    // Reset Graph State

    Audio::play_reset();
    // Play reset information for user FULL_STOP!!
    this->goal_progression = 0;
    this->goal_list.clear();

   locator_graph.reset_graph();

    return 1;
}

static void sleep_loc(int n) {
   std::chrono::seconds timespan(n);
   std::this_thread::sleep_for(timespan);
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
std::vector<cardinalDirection> Locator::check_openings(Sonar_Distances & distances) {

    std::vector<cardinalDirection> directions = {DIR_N, DIR_E, DIR_S, DIR_W};

    std::cout << "The right value was: " << this->curr_cycle.right_distance<< std::endl;
    std::cout <<  "The left value was: " <<this->curr_cycle.left_distance << std::endl;
    std::cout <<  "The front value was: " <<this->curr_cycle.forward_distance << std::endl;

    float max_l = curr_cycle.left_distance;
    float max_r = curr_cycle.right_distance;

    for (int i = 0; i < 10; i++) {

        read_distances();
        sleep_loc(1);

        if (curr_cycle.right_distance > max_r) {
            max_r = curr_cycle.right_distance;
        }

        if (curr_cycle.left_distance > max_l) {
            max_l = curr_cycle.left_distance;
        }
    }

    if (max_l < INTERSECTION_THRESHOLD) {

        directions[LEFT] = INVALID_DIRECTION;
    }

    if (max_r < INTERSECTION_THRESHOLD) {

        directions[RIGHT] = INVALID_DIRECTION;
    }

    if (curr_cycle.forward_distance < 80) {

        directions[FORWARD] = INVALID_DIRECTION;
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

    if (this->curr_cycle.right_distance > INTERSECTION_THRESHOLD || this->curr_cycle.left_distance > INTERSECTION_THRESHOLD*1.5) {

        std::cout << "The right value was: " << this->curr_cycle.right_distance<< std::endl;
        std::cout <<  "The left value was: " <<this->curr_cycle.left_distance << std::endl;
        std::cout <<  "The front value was: " <<this->curr_cycle.forward_distance << std::endl;
        retV = INTERSECTION;
    }

    if (this->curr_cycle.forward_distance < FRONT_TRESHOLD && this->curr_cycle.forward_distance > 12) {

        std::cout << "The right value was: " << this->curr_cycle.right_distance<< std::endl;
        std::cout <<  "The left value was: " <<this->curr_cycle.left_distance << std::endl;
        std::cout <<  "The front value was: " <<this->curr_cycle.forward_distance << std::endl;

        retV = INTERSECTION;
    }

    return retV;
};

void Locator::read_distances() {
    this->newest_metrics.start_read();

    this->curr_cycle.right_distance  = newest_metrics.read(RIGHT_DISTANCE);
    this->curr_cycle.left_distance = newest_metrics.read(LEFT_DISTANCE);
    this->curr_cycle.forward_distance = newest_metrics.read(FORWARD);


    this->newest_metrics.end_read();
}

void Locator::run_locator() {

    this->read_distances();

    // Rising edge for ceiling light detection
    old_light_res = light_res;
    old_intersection = intersection;

    light_res = proc.step_detect(this->camera, intersection);
    std::cout << "Checking light\n";
    locatorState new_light = (light_res ^ old_light_res) & light_res;

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
        locator_graph.edge_step();
        Audio::play_light();
    }
}


void Locator::intersection_verify(detectionResult intersect, detectionResult old_intersect) {

    if ((intersect ^ old_intersect) & intersect) {
        Audio::intersection();

        // Prompt user that he has reached intersection

        cardinalDirection to_turn;


        if (goal_progression) {
            if (goal_list.size() > 0) {
                to_turn = goalDirection();


                handDirection turn_prompt = Graph_Utils::cardinal_to_hand(to_turn, curr_heading);
                curr_heading = to_turn;

                Audio::turn_dir(turn_prompt);
            } else {
                // At this point we should be at goal, in case we are not we reset after 10 seconds
                sleep_loc(15);

                this->reset_state();
            }
        } else {

            to_turn = standardDirection(to_turn);

            if (locator_graph.path_count() == 1 && !goal_progression) {

                goal_setup();
                to_turn = goalDirection();
                handDirection turn_prompt = Graph_Utils::cardinal_to_hand(to_turn, curr_heading);
                curr_heading = to_turn;

            sleep_loc(1);

                Audio::turn_dir(turn_prompt);
            } else {

             sleep_loc(1);
                Audio::turn_dir(to_turn);
            }
        }
       sleep_loc(5);
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
    // Update Distances
    this->read_distances();

    // Check openings based on most recent read
    std::vector<cardinalDirection> openings = check_openings(this->curr_cycle);

    // Get turn in Open direction
    to_turn = this->next_step(openings);

    // Update the graph state to correspond with node we are at
    bool result = locator_graph.intersection_update(openings);

    // If no paths were advanced, then state is inconsistent; reset graph
    if (!result) {
        this->reset_state();
    }

    return to_turn;
}

cardinalDirection Locator::goalDirection() {
    cardinalDirection to_turn = INVALID_DIRECTION;
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
        con = new Arduino_Connector(&this->newest_metrics, data_source, ARDUINO);

        // Initialize connection with arduino
        con->init_connection();

        // Start thread to read packets from connection
        con->start_thread();
        retv = 1;


    } else if (source_type == SIMULATED_DATA) {
        con = new Arduino_Connector(&this->newest_metrics, data_source, SIMULATION);
        con->start_thread();
        retv =  1;
    }

    Audio::play_start();
    sleep_loc(10);

    return retv;
}

Locator::Locator(std::string file_uri, int run_type) {

    this->proc = Image_Processor();

    if (run_type == ARDUINO) {
        #ifdef __arm__
         this->camera.config_connector(Camera_Type::RASPBERRY_PI_CAM_E, file_uri, 0);
        #else
        this->camera.config_connector(Camera_Type::USB_WEBCAMS_E , file_uri, DEFAULT_CAMERA);
        #endif

    } else if (run_type == SIMULATION) {
        this->camera.config_connector(Camera_Type::IMAGE_FOLDER_E, file_uri, 180);
    }

    // Set Light and intersection indicators to 0
    this->light_res = 0;
    this->old_light_res = 0;
    this->intersection = 0;
    this->old_intersection = 0;
}

Locator::~Locator() {
    int end = 0;
    do {
        end = con->stop_thread();
    } while (!end);

    con->end_connection();
}