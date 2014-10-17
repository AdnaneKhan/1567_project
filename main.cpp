
#include "camera_connector.h"

#include <stdlib.h>

int main() {

    cv::VideoCapture cap;

    int i;
    for (i = 2; i < 1500; i++ ) {
        if (cap.open(i)){
            std::cout << "Found Camera " << i;
            break;
        }

    }

    std::string init = "val";
    Camera_Connector * connect = new Camera_Connector(USB_WEBCAM,init);


    connect->test_image();

    return 0;
}

