#include "locator.h"


Locator::Locator(std::string file_uri) {
    ofstream myfile;
    Locator::camera = new Camera_Connector;

    myfile.open (file_uri, ios::in);
    std::string line;
    if (myfile.is_open())
    {
        // First line is num of nodes
        line = getline(myfile,line);
        int count = line.c_str();

        Locator::graph = Node[count];

        // Populate graph with nodes
        while ( getline (myfile,line) && count > 0)
        {
            // Split to get node id

            // "iterate" through ids of neighbors and add their ids + costs to the graph


            count--;
        }
        myfile.close();
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
