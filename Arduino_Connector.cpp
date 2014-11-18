#include "Arduino_Connector.hpp"

Arduino_Connector::Arduino_Connector(Arduino_Packet * data_in, std::string serial_info) {

    this->data_holder = data_in;

    this->serial_id = this->init_serial(serial_info);
}


int Arduino_Connector::init_serial(std::string serial_info) {
    int id;

    // Set configuration parameters


    // Initialize the serial read
    id = open(serial_info.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);


    return id;
}

void Arduino_Connector::start_thread() {
    this->thread_halt = FALSE;
    arduino_connection = std::thread(&Arduino_Connector::serial_read, this, serial_id);
}

int Arduino_Connector::stop_thread() {
    this->thread_halt = TRUE;

    if (arduino_connection.joinable()) {
        arduino_connection.join();
        return 1;
    } else {
        return 0;
    }
}


void Arduino_Connector::parse_packet(char * string_in,int buf_max, Arduino_Packet & to_update) {

    int val_count = 0;
    int start = 0;
    char * endPtr;

    for (int i = 0; i < buf_max; i++) {

        // Mark the close of the float
        if ( string_in[i] == ',' || string_in[i] == '}' ) {
            endPtr = &string_in[i];

            float parsed_value = strtof((string_in + start), &endPtr);
            to_update.values[val_count] = parsed_value;
            val_count++;
        }
    }
}

int Arduino_Connector::serial_read(int serial_handle) {

    int string_counter = 0;

    while (this->thread_halt == FALSE) {
        char byte_in;
        // Read 1 byte
        int readCount = read( serial_handle , &byte_in, 1);


        if (readCount > 0) {
            // Reached end of line
            if ( byte_in == '\n') {
                this->buffer[string_counter ] = '\n';
                parse_packet(this->buffer, string_counter, *this->data_holder );
                string_counter = 0;


                std::chrono::milliseconds dura( 250 );
                std::this_thread::sleep_for(dura);

            } else {
                this->buffer[string_counter] = byte_in;
            }

        }
    }

    return 0;
}

void Arduino_Connector::init_connection() {
}

int Arduino_Connector::end_connection() {
    char stop = STOP_SENTINEL;
    int writeout = write(serial_id, &stop, 1);

    close(serial_id);

    return writeout;
}