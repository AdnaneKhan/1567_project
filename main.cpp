#define DEBUG


#include "locator.h"


int main() {

    std::string init = "val";


    Locator *loc = new Locator(" ", "/dev/tty.usbmodem1411");
    std::chrono::milliseconds timespan(500);
    int res = loc->start("/dev/tty.usbmodem1411");
    if (res) {
        while (!loc->is_located()) {
            loc->run_locator();
            std::this_thread::sleep_for(timespan);
        }
    }

    delete(loc);
//    Image_Processor proc = Image_Processor();
//    Camera_Connector camera = Camera_Connector(USB_WEBCAM, "", 0);
//    Mat img = camera.get_image();
//
//    proc.hough_rectangle_detect(img);
//

//    int intersection;
//
//    int res = proc.step_detect(camera, intersection);
//
//    std::cout << " The result was " << intersection << std::endl;

    return 0;
}

