#include "Camera_Connector.h"


using namespace cv;

class camera_exception : public std::exception {
    virtual const char *what() const throw() {
        return "The camera ID specified was not valid";
    }
} camera_ex;

/**
Returns new image from camera source, if the
source is a folder then reads new file from list of file names
and returns that, if it is an actual camera then returns from pre-initialized reader object
*/
cv::Mat Camera_Connector::get_image() {
    cv::Mat ret_image;

    if (camera_source == IMAGE_FOLDER) {
        if (f_name_queue.size() > 0) {
            std::string file_name = f_name_queue.front();
            f_name_queue.pop();

            cv::VideoCapture file_in(file_name);
            file_in.read(ret_image);
        } else {
            // No more files from source, throwing exception
            throw camera_ex;
        }

    } else if (camera_source == RASPBERRY_PI_CAM) {
    #ifdef __arm__
       Camera.grab();
       std::cout << "We just grabbed an image from raspberry_Pi cam\n" << std::flush;
       Camera.retrieve(ret_image);
    #endif
    } else {
        cam.read(ret_image);
    }

#ifdef DEBUG
        Camera_Connector::write_image("recent_image", ret_image);
    #endif
    return ret_image;
}

/**
* \param to_init object that will hold initiliazed camera
* \param camera_id id of camera to initialize, will very with computer
* \throws camera_ex if the camera was not able to be found

*/
void usb_camera_init(cv::VideoCapture &to_init, int camera_id) {

    if (to_init.open(camera_id)) {
        std::cout << "Found Camera " << camera_id << '\n';
        std::chrono::milliseconds timespan(500);
        to_init.set(CV_CAP_PROP_AUTO_EXPOSURE,0);
        to_init.set(CV_CAP_PROP_GAIN,0);
        std::this_thread::sleep_for(timespan);
    } else {
        // Failed to initialize camera
        throw camera_ex;
    }

}

#ifdef __arm__
int pi_camera_init( raspicam::RaspiCam_Cv & to_init ) {
    to_init.set(CV_CAP_PROP_FORMAT, CV_8UC1 );

    if (!to_init.open()) {
        // Problem
        std::cout << "The Camera was unable to be opened!\n";
    }
}
#endif

void Camera_Connector::close_camera() {
    if(camera_source == RASPBERRY_PI_CAM) {
        #ifdef __arm__
           Camera.release();
        #endif
    } else {
        cam.release();
    }
}

void Camera_Connector::write_image(std::string filename, cv::Mat &img) {
    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    std::cout << "Writing image, the size is " << img.size()<<std::endl;

    cv::imwrite(WRITE_LOCATION + filename + ".png", img, compression_params);
}

/**
* \param camera_id of the camera to initialzie
* \param source of the folder
* \param camera_source which source to use (USB, FILE, RASPI_CAM)
*
*     1 - RASPBERRY_PI_CAM
*     2 - USB_WEBCAM
*     3 - IMAGE_FOLDER
*/
Camera_Connector::Camera_Connector(int camera_source, std::string source, int camera_number) {
    Camera_Connector::camera_source = camera_source;
    switch (camera_source) {
        case USB_WEBCAM:
            usb_camera_init(cam, camera_number);

            break;
#ifdef __arm__
        case RASPBERRY_PI_CAM:

            // Initialize pi_cam
            pi_camera_init(Camera);
            break;
#endif
        case IMAGE_FOLDER:
            this->file_folder = source;
            // Note that all of the file names are in a defined format

            for (int i = 0; i < camera_number; i++) {
                // Push file names for test files TODO: make number flexible
                this->f_name_queue.push(source + "test"+ std::to_string(i) +".png"  );
            }

            break;
        default:
            throw camera_ex;
    }
}

Camera_Connector::~Camera_Connector() {
    close_camera();
}