#define DEBUG

#include "Locator.h"

#define SIMULATION 2
#define REAL 1
#define INVALID 3

typedef int operationType;

operationType select_operation( int num_args,char * args[]) {
    operationType to_return;
    if (num_args == 4 ) {
        to_return = SIMULATION;
    } else if(num_args == 3 ) {
        to_return = REAL;
    } else {
        to_return = INVALID;
    }

    return to_return;
}

/*
 Reads images from the image folder (instead of camera) and data from a text file
 with the path mock_data_file (instead of from the arduino.
 */
void run_simulation(const char * image_folder, const char * mock_data_file) {
    Locator * loc = new Locator(image_folder, SIMULATION);
    std::chrono::milliseconds timespan(500);
    int res = loc->start(mock_data_file, SIMULATED_DATA);

    if (res) {
        while(true) {
            loc->run_locator();
            std::this_thread::sleep_for(timespan);
        }
    } else {
        std::cout << "There was a problem with initializing the locator for simulation\n";
    }
}

/*
Runs the actual locater using data from camera and arduino serial connection
 */
void run_full(const char * serial_port) {
    Locator * loc = new Locator("", ARDUINO);
    std::chrono::milliseconds timespan(250);
    int res = loc->start(serial_port, ARDUINO_DATA);


    std::this_thread::sleep_for(timespan);
    if (res) {
        while (true) {
            loc->run_locator();
        }
    } else {
        std::cout << "There was a problem with initializing the locator.\n";
    }
}

int main(int argc, char * argv[]) {

    if (select_operation(argc, argv) == REAL) {
        run_full(argv[2]);
    } else if (select_operation(argc, argv) == SIMULATION) {
        run_simulation(argv[2], argv[3]);
    } else {
        std::cout << "Proper command arguments were not specified, exiting.\n";
    }

    return 0;
}

