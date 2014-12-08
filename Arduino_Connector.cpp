#include "Arduino_Connector.hpp"


Arduino_Connector::Arduino_Connector(Arduino_Packet *data_in, std::string source_info, int connection_type) {
    this->data_holder = data_in;

    if (connection_type == ARDUINO) {
        this->type = ARDUINO;
        this->serial_id = this->init_serial(source_info);
    } else if (connection_type == SIMULATION) {
        this->type = SIMULATION;
        this->data_file_name = source_info;
    }
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
    if (type == SIMULATION) {
         arduino_connection = std::thread(&Arduino_Connector::file_read,this,data_file_name);
    } else if (type == ARDUINO) {
        arduino_connection = std::thread(&Arduino_Connector::serial_read, this, serial_id);
    }
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

    for (int i = 0; i < buf_max; i++) {

        // Mark the close of the float
        if (( string_in[i] == ',' || string_in[i] == '}') && start) {

            float parsed_value = strtof((string_in + start), nullptr);


            to_update.update(val_count, parsed_value);
            val_count++;
            start = 0;
        }

        if ((string_in[i] == '{' || string_in[i] == ',') && !start ) {
            start = 1 + i;

        }
    }
}

int Arduino_Connector::file_read(std::string data_source) {
    std::ifstream file_in(data_source);
    std::string line;
    char * string_buf;

    std::chrono::milliseconds dura( 500 );


    if (file_in.is_open()) {

        while (this->thread_halt == FALSE && getline (file_in,line)) {
            string_buf = const_cast<char*>(line.c_str());
            parse_packet(string_buf, line.length() - 1, *this->data_holder);
            std::this_thread::sleep_for(dura);
        }
    }

    return 0;
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
                this->buffer[string_counter] = 0;
                parse_packet(this->buffer, string_counter, *this->data_holder );

                #ifndef __arm__
                write_packet(this->buffer,string_counter);
                #endif
                string_counter = 0;

                #ifdef DEBUG
                    std::cout << this->buffer << "\n";
                #endif
            } else {
                this->buffer[string_counter++] = byte_in;
            }
        }
    }

    return 0;
}

#ifndef __arm__
void Arduino_Connector::open_file_w() {
    this->w_file = std::ofstream("/Users/adnankhan/Box Sync/Robots/1567_project/test_data/dummy.txt");
}


void Arduino_Connector::write_packet(char * string_in,int buf_max) {
    w_file << string_in;

}
#endif
void Arduino_Connector::init_connection() {
    char start = START_SENTINEL;
this->open_file_w();
#ifndef __arm__
    int writeout = write(serial_id, &start, 1);
#endif
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