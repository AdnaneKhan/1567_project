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
#include "Audio.h"

#define ARDUINO 1
#define SIMULATION 2

#define STOP_SENTINEL '#'
#define START_SENTINEL '$'

#define TRUE 1
#define FALSE 0

#define FRONT_DISTANCE 0
#define FRONT_LEFT 1
#define LEFT_DISTANCE  2
#define FRONT_RIGHT 3
#define RIGHT_DISTANCE 4
#define DOWN_SONAR 5
#define HEADING_OK 6
#define DIST_OK 7

//#define DEBUG
typedef float sensorDistance;
typedef float sensorValue;


typedef struct Arduino_Packet
{
    std::mutex mutex;

private:

    struct Values
    {
        sensorDistance l_distance;
        sensorDistance l_2_distance;
        sensorDistance front_distance;
        sensorDistance r_2_distance;
        sensorDistance r_distance;
        sensorValue down_sonar;
        sensorValue heading_ok;
        sensorValue dist_ok;

    } Values;

public:
    void update(int value_select, sensorValue new_val)
    {

        mutex.lock();
        switch (value_select)
        {
            case LEFT_DISTANCE:
                Values.l_distance = new_val;
                break;
            case FRONT_LEFT:
                Values.l_2_distance = new_val;
                break;
            case FRONT_DISTANCE:
                Values.front_distance = new_val;
                break;
            case FRONT_RIGHT:
                Values.r_2_distance = new_val;
                break;
            case RIGHT_DISTANCE:
                Values.r_distance = new_val;
                break;
            case DOWN_SONAR:
                Values.down_sonar = new_val;
                break;
            case HEADING_OK:
                Values.heading_ok = new_val;
                break;
            case DIST_OK:
                Values.dist_ok = new_val;
                break;
        }
        mutex.unlock();
    }

    //// Read must be used before reading to ensure values are consistent with packet
    void start_read()
    {
        mutex.lock();
    }

    void end_read()
    {
        mutex.unlock();
    }
    /////

    sensorValue read(int value_select)
    {

        sensorValue to_return = 0;

        switch (value_select)
        {
            case LEFT_DISTANCE:
                to_return = Values.l_distance;
                break;
            case FRONT_LEFT:
                to_return = Values.l_2_distance;
                break;
            case FRONT_DISTANCE:
                to_return = Values.front_distance;
                break;
            case FRONT_RIGHT:
                to_return = Values.r_2_distance;
                break;
            case RIGHT_DISTANCE:
                to_return = Values.r_distance;
                break;
            case DOWN_SONAR:
                to_return = Values.down_sonar;
                break;
            case HEADING_OK:
                to_return = Values.heading_ok;
                break;
            case DIST_OK:
                to_return = Values.dist_ok;
                break;
        }


        return to_return;
    }

    Arduino_Packet()
    {
        this->Values = {0};
    }

} Arduino_Packet;

class Arduino_Connector
{

public:

    void middle_check();

    /*
        \param data_in pointer to Arduino Packet which has latest data filled into if
     */
    Arduino_Connector(Arduino_Packet *data_in, std::string source_info, int connection_type);

    // Initializes the serial connection with Arduino
    void init_connection();

    // Closes serial connection with arduino
    int end_connection();

    // Starts thread reading from Arduino
    void start_thread();

    // Stops the thread connecting to Arduino
    int stop_thread();

private:

#ifndef __arm__

    // Writes packet to file if we are not running on Raspberry Pi
    void write_packet(char *string_in, int buf_max);

#endif

    // Parses a packet in the following format:
    // {val1_val2_val3_val4_val5_val6_val7_val8}
    // Where indixes coresspond to values in recent data
    void parse_packet(char *string_in, int buf_max, Arduino_Packet &to_update);

    // Reads packets from a text file instead of Arduino serial
    // for running simulations
    int file_read(std::string data_source);

    // Reads data from serial
    int serial_read(int serial_handle);

    // name of data file for simulation reading
    std::string data_file_name;

    // Initialize connection with Arduino on given serial port
    int init_serial(std::string serial_info);

    // Type of connection (simulation or arduino read)
    int type;

    Arduino_Packet *data_holder;

    // Buffer to hold string from Arduino
    char buffer[200];

    // ID of serial connection
    int serial_id;

    // Indicates that the thread needs to stop
    // used when ending connection
    int thread_halt;

    int aud_tim;

    // If we are NOT running on Raspberry Pi then file open to write packet data to
#ifndef __arm__
    std::ofstream w_file;

    void open_file_w();

#endif

    // Runs the arduino connection as a seperate thread to receive packets from
    // arduino over serial USB.
    std::thread arduino_connection;
};

#endif