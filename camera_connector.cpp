#include "camera_connector.h"

class camera_exception: public std::exception {
    virtual const char* what() const throw() {
        return "The camera ID specified was not valid";
    }
} camera_ex;


void Camera_Connector::test_image() {
    cv::Mat camera_test;
    get_image(camera_test);

    cv::imshow("Test Image", camera_test);

    return;
}

int Camera_Connector::process_image(cv::Mat & rect_queue ) {

    cv::Mat camera_cap;
    get_image(camera_cap);


    // Rectangle is now normalized to 2D view
    cv::Mat camera_result = this->perspective_adjust(camera_cap);

    // Get following information about rectangle:
    // center

    // Lock Mutex

    // Place Result in queue

    // Unlock Mutex

    return 0;
}


/*
 Returns new image from camera source, if the
 source is a folder then reads new file from list of file names
 and returns that, if it is an actual camera then returns from pre-initialized reader object
 */
int Camera_Connector::get_image(cv::Mat & image_ret) {

    if (camera_source == IMAGE_FOLDER) {
        std::string file_name;

        cv::VideoCapture file_in(file_name);
        file_in.read(image_ret);
    } else if (camera_source == RASPBERRY_PI_CAM) {
//        Camera.retrieve(image_ret);
    } else {
        cam.read(image_ret);
    }

    return 0;
}

/*

Citing https://github.com/bsdnoobz/opencv-code/blob/master/quad-segmentation.cpp

 */
cv::Mat Camera_Connector::perspective_adjust(cv::Mat & source) {
    cv::Mat processed_image;




    return processed_image;
}

int usb_camera_init(cv::VideoCapture & to_init ) {

    int i;
    for (i = 0; i < 1500; i++ ) {
        if (to_init.open(i)){
            std::cout << "Found Camera " << i << '\n';
            std::chrono::milliseconds timespan(500);
            std::this_thread::sleep_for(timespan);
            break;
        }
    }

    return 0;
}

#ifdef __arm__
int pi_camera_init( raspicam::RaspiCam_Cv & to_init ) {
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1 );

    if (!Camera.open()) {
        // Problem
    }
}
#endif

Camera_Connector::Camera_Connector(int camera_source, std::string source) {
    switch (camera_source){
        case USB_WEBCAM:
            usb_camera_init(cam);

            break;
            #ifdef __arm__
        case RASPBERRY_PI_CAM:

            // Initialize pi_cam
            break;
            #endif
        case IMAGE_FOLDER:
            // Initialize file reader
        Camera_Connector::camera_source = camera_source;
            break;
        default:
            throw camera_ex;
            break;
    }

}

Camera_Connector::~Camera_Connector() {
    cam.~VideoCapture();

}