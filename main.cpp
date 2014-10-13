#ifndef CAMERA_INC
#include ""e "camera_connector.h"
#define CAMERA_INC
#endif



int main() {
    std::cout << "Hello, World!" << std::endl;

    cv::VideoCapture cap;

    int i;
    for (i = 0; i < 1500; i++ ) {
        if (cap.open(i)){
            std::cout << "Found Camera %d\n" << i;
        }

    }

    return 0;
}

