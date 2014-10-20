#include "locator.h"

bool Locator::is_located() {
    bool retb = false;

    // If all possible paths reduced to 1, then we have found our location
    if (step_lists.size() == 1) {
        retb = true;
    }

    return retv;
}

Locator::Locator(std::string file_uri) {
    ofstream myfile;
    this->camera = new Camera_Connector(USB_WEBCAM,init);
    this->edge_progress = 0;
    step_lists.resize(12);

    char nodes[12] = {'A','B','C','D','E','F','G','H','I','J','K','L'};
    char edges[13][3] = { {'A','B',0, 0},{'A','F',0,0},{'F','G',0,0},{'G','H',0,0},{'H','I',0,0},{'H','K',0,0},{'I','J',0,0},{'J','K',0,0},{'K','B',0,0},{'B','C',0,0},{'C','D',0,0},{'E','D',0,0},{'E','L',0,0}};

    for (int i = 0; i < 12; i++) {
        // set initial steps
        step_lists.at(i).push_back(i + CHAR_TO_POSITION);
        sennot_graph.nodes.at(i) = Node(i + CHAR_TO_POSITION);
    }

    // Add all edges in graph
    for (int i = 0; i < 13; i++) {
        sennot_graph.nodes.at(edges[i][0] - 65).add_neighbor(edges[i][1], edges[i][2], edges[i][3]);
    }

}


int Locator::graph_step() {

    edge_progress++;

//    for (int i = 0; i < step_lists.size(); i++) {
//
//    }
    // for paths in possible paths
    // step each by one
    // return number of valid paths we can still traverse
}

void Locator::graph_intersect(int step_count) {



    // reset progress
    edge_progress = 0;
}

Locator::run_locator() {

    int intersection = 0;
    int old_res = 0;
    int res = 0;

    int step;
        get_image(camera_test);
        old_res = res;
        res = step_detect(camera_test,intersection);
        step = (res ^ old_res) & res;
        if (step) {
            ///
            Loactor::grapth_step();
            if (intersection) {
                // We are at intersection, check to see which paths we could possibly be on
                Locator::graph_intersect(edge_progress);
            }
            if (graph_step() == 1) {
                // Paths have been narrowed to one

            }
        }
}

Locator::~Locator() {
    delete(Locator::camera);
    thread_halt = TRUE;

    if (arduino_connection.joinable()) {
        arduino_connection.join();
    }
}


int Locator::start(std::string serial_info, std::string receive_data) {
    int serial_id;
    int retv;
    // Initialize the serial read

    serial_id = open(serial_info, O_RDONLY | O_NOCTTY | O_NDELAY );




    if (serial_id != -1) {
        Locator::thread_halt = FALSE;
        arduino_connection(receive_data,serial_info);

        retv = TRUE;
    } else { // Serial port was not opened successfully
        retv = FALSE;
    }

    return retv;
}

void Locator::receive_data(int serial_id) {
    Arduino_packet read = {0};

    while (!thread_halt) {
        // Treat read as a buffer and read values to it.

        int byte_count = read(serial_id, &read, sizeof(Arduino_packet));
        cout << "Read % bytes into the Arduino_packet" << byte_count;

        if (byte_count == sizeof(Arduino_packet)) {
            this->recent_metrics.back_distance = read.back_distance;
            this->recent_metrics.front_distance = read.front_distance;
            this->recent_metrics.heading = read.heading;
            this->recent_metrics.l_distance = read.l_distance;
            this->recent_metrics.r_distance = read.r_distance;
        }

        //Sleep for 100 ms before reading next
        std::chrono::milliseconds dura( 100 );
        std::this_thread::sleep_for( dura );

    }

    close(serial_id);
}
