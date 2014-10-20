

#include "locator.h"

#include <stdlib.h>

int main() {

    std::string init = "val";
    Camera_Connector * connect = new Camera_Connector(USB_WEBCAM,init);


    connect->test_image(10);

    return 0;
}

