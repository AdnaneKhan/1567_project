#include "Audio.h"
#include <iostream>

namespace Audio {

    // Currently methods stubbed with calls to print
    // Playing actual sounds is to be implemented once all logic has been worked out

    void play_left() {
        std::cout << "Please turn left." << std::endl;
    }

    void play_right() {
        std::cout << "Please turn right." << std::endl;
    }

    void play_forward() {
        std::cout << "Please move forward." << std::endl;
    }

    void play_back() {
        std::cout << "Please turn around." << std::endl;
    }

    void turn_progress(int curr_degrees, int target_degrees) {
        std::cout << "You have turned " << curr_degrees << " degrees and you need to turn " << (target_degrees - curr_degrees) << " degrees." << std::endl;
    }

    void turn_complete() {
        std::cout << "You have succesfully completed your turn." << std::endl;
    }

    void intersection() {
        std::cout << "You have arrived at an intersection." << std::endl;
    }

    void turn_dir(int dir) {
        switch (dir) {
            case 0:
                play_forward();
                break;
            case 1:
                play_right();
                break;
            case 2:
                play_back();
                break;
            case 3:
                play_left();
                break;
            default:
                // No-op
                break;
        }
    }

    void play_light() {
        std::cout << "You have walked under a light." << std::endl;
    }
}