#define DEBUG


#include "Locator.h"


int main() {

    std::string init = "val";
#ifdef __arm__
        system("omxplayer -o local audio_prompts/left.mp3");
        #else
    system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/begun.m4a");
#endif

    Locator *loc = new Locator(" ", "/dev/tty.usbmodem1451");
    std::chrono::milliseconds timespan(500);
    int res = loc->start("/dev/tty.usbmodem1451");
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

