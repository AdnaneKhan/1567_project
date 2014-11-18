#ifndef ARDUINO_CONNECTOR_H
#define ARDUINO_CONNECTOR_H

#include <thread>
#include <string>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <iostream>
#include <fstream>

typedef float sensorDistance;

#define STOP_SENTINEL '#'
#define START_SENTINEL '$'

#define TRUE 1
#define FALSE 0

typedef union Arduino_Packet {
    float values[8];
    struct Values {
        sensorDistance l_distance;
        sensorDistance r_distance;
        sensorDistance back_distance;
        sensorDistance front_distance;
        float heading;
        float x_tilt;
        float y_tilt;
        float z_tilt;
    } Values;


} Arduino_Packet;


class Arduino_Connector {

public:


    void init_connection();
    int end_connection();

    Arduino_Connector(Arduino_Packet * data_in, std::string serial_info);

    void start_thread();
    int stop_thread();
private:
    void parse_packet(char * string_in,int buf_max, Arduino_Packet & to_update);

    int serial_read(int serial_handle);
    int init_serial(std::string serial_info);
    char buffer[200];
    Arduino_Packet * data_holder;

    int serial_id;
    int thread_halt;
    std::thread arduino_connection;
};


#endif