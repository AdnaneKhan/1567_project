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

     #ifdef DEBUG
    std::cout << id << std::endl;
    #endif

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

    #ifdef DEBUG
        std::cout << "About to parse " << string_in << " to " << buf_max <<std::endl;
    #endif

    for (int i = 0; i < buf_max; i++) {

        // Mark the close of the float
        if (( string_in[i] == '_' || string_in[i] == '}') && start) {


            float parsed_value = strtof((string_in + start), nullptr);

            #ifdef DEBUG
                 std::cout << "Just parsed:" << parsed_value << " ";
            std::cout << start << " to " << i-1 <<std::endl;
            #endif
            to_update.update(val_count, parsed_value);
            val_count++;
            start = 0;
        }

        if ((string_in[i] == '{' || string_in[i] == '_') && !start ) {
            start = 1 + i;

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

                #ifdef DEBUG
                    std::cout << "Newline read\n";
                #endif

                parse_packet(this->buffer, string_counter, *this->data_holder );
                string_counter = 0;


                #ifdef DEBUG
                    std::cout << this->buffer << "\n";
                #endif

                std::chrono::milliseconds dura( 250 );
                std::this_thread::sleep_for(dura);

            } else {
                this->buffer[string_counter++] = byte_in;
            }
        }
    }

    return 0;
}

void Arduino_Connector::init_connection() {
    char start = START_SENTINEL;

    int writeout = write(serial_id, &start, 1);

   #ifdef DEBUG
        std::cout << "Wrote " << writeout << std::endl;
    #endif
}

int Arduino_Connector::end_connection() {
    char stop = STOP_SENTINEL;
    int writeout = write(serial_id, &stop, 1);

    close(serial_id);

    return writeout;
}