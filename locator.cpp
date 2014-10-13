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



    }

    close(serial_id);
}
