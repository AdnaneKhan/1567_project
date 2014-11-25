#include "Locator.h"

locatorState Locator::reset_state() {
    // Reset Graph State

    return 1;
}

/**
*  \return retb true/false whether the locator has reduced set of traversals to one
*/
bool Locator::is_located() {
    bool retb = false;

    int path_num = this->locator_graph.path_count();

    if (path_num == 0) {
        // Problem state
    }

    // If all possible paths reduced to 1, then we have found our location
    if (path_num == 1) {
        retb = true;
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
int Locator::check_openings(Arduino_Packet &packet, std::vector<int> &directions, cardinalDirection curr_direction) {
    int retV = 0;

    if (packet.Values.l_distance < INTERSECTION_THRESHOLD) {
        directions[(3 + curr_direction) % 4] = -1;
        retV = 1;
    }

    if (packet.Values.r_distance < INTERSECTION_THRESHOLD) {
        directions[(1 + curr_direction) % 4] = -1;
        retV = 1;
    }

    if (packet.Values.front_distance < INTERSECTION_THRESHOLD) {
        directions[curr_direction] = -1;
        retV = 1;
    }

    // Back is is open since that is where we came from
    directions[(2 + curr_direction) % 4] = 1;


    return retV;
}


cardinalDirection Locator::convert_dir(cardinalDirection to_convert, int current_heading) {
    cardinalDirection new_dir;

    new_dir = (current_heading + to_convert) % 4;

    return new_dir;
}


cardinalDirection Locator::next_step_m() {

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
            return directions[i] / 2;
        }
    }

  reset_state();

    return 0;
}


/**
Based on possible possible locations decide which direction to have user turn

@packet most recent metrics acquired from analog sensors

returns direction to turn (value from 0 to 3), -1 if no valid turns found
this indicates that locator is in an unknown state and re-setting may be necessary.
*/
cardinalDirection Locator::next_step(Arduino_Packet &packet) {
    cardinalDirection to_turn = Graph_Utils::parse_direction(packet.read(HEADING));

    std::vector<cardinalDirection> directions = {N, E, S, W};

    // remove directions that are not open
    check_openings(packet, directions, to_turn / 6);

    int opposite_dir = (to_turn + 4) % 8;
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

void Locator::run_locator() {

    graphInt step;


    // Rising edge for ceiling light detection
    old_res = res;
    old_intersection = intersection;

    res = proc.step_detect(this->camera, intersection);

    locatorState new_light = (res ^ old_res) & res;

    // Need to ensure we don't count circles as lights too
    if ((intersection ^ old_intersection) & intersection) {



        // Prompt user that he has reached intersection
        Audio::intersection();


        if (locator_graph.path_count() == 1) {
            this->last_node = locator_graph.get_last_node();

            if (!goal_progression) {

                this->goal_list = locator_graph.find_path(last_node, GOAL_NODE);

                goal_progression = TRUE;
            }
        }
        cardinalDirection to_turn;

        if (this->goal_progression) {
            // Check connection between current and front of goal list
            to_turn = Graph_Utils::check_connection(last_node, this->goal_list.front() , this->locator_graph);

        } else {
            to_turn = this->next_step_m();
        }

        // We are at intersection, check to see which paths we could possibly be on, if we
        // have narrowed to one and located, then this serves as navigation
        cardinalDirection dir = this->locator_graph.graph_intersect(to_turn);


        cardinalDirection turn_command = convert_dir(turn_command, this->curr_heading);
            #ifdef DEBUG
                    std::cout << "The path was " << dir << std::endl;
            #endif
        Audio::turn_dir(turn_command);
    }

    // Need
    if (new_light) {
        Audio::play_light();
        ///
        locator_graph.graph_step();
    }
}


int Locator::start(std::string serial_info) {
    int serial_id;
    int retv;
    // Initialize the serial read

    con = new Arduino_Connector(&this->recent_metrics, serial_info);

    con->init_connection();

    con->start_thread();

    return retv;
}


Locator::Locator(std::string file_uri, std::string serial_id) {
    std::ofstream myfile;

    this->proc = Image_Processor();
    this->camera = Camera_Connector(USB_WEBCAM, "", DEFAULT_CAMERA);

    this->res = 0;
    this->old_res = 1;
    this->intersection = 0;
    this->old_intersection = 0;


}

Locator::~Locator() {

    int end = 0;
    do {
        end = con->stop_thread();
    } while (!end);


}