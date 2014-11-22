#ifndef ARDUINO_CONNECTOR_H
#define ARDUINO_CONNECTOR_H

#include <thread>
#include <mutex>
#include <string>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <iostream>
#include <fstream>

typedef float sensorDistance;
typedef float sensorValue;

#define STOP_SENTINEL '#'
#define START_SENTINEL '$'

#define TRUE 1
#define FALSE 0

typedef struct Arduino_Packet {
std::mutex mutex;


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

    void update(int value_select, sensorValue new_val) {

        mutex.lock();
        switch(value_select) {
            case 0: Values.l_distance = new_val; break;
            case 1: Values.r_distance = new_val; break;
            case 2: Values.back_distance = new_val; break;
            case 3: Values.front_distance = new_val; break;
            case 4: Values.heading = new_val; break;
            case 5: Values.x_tilt = new_val; break;
            case 6: Values.y_tilt = new_val;  break;
            case 7: Values. z_tilt = new_val; break;
        }
        mutex.unlock();
    }

    sensorValue read(int value_select) {

        sensorValue to_return = 0;
        mutex.lock();
        switch(value_select) {
            case 0: to_return = Values.l_distance; break;
            case 1: to_return = Values.r_distance; break;
            case 2: to_return = Values.back_distance; break;
            case 3: to_return = Values.front_distance; break;
            case 4: to_return = Values.heading; break;
            case 5: to_return = Values.x_tilt; break;
            case 6: to_return = Values.y_tilt;  break;
            case 7: to_return = Values. z_tilt; break;
        }
        mutex.unlock();

        return to_return;
    }


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