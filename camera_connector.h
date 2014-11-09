#ifndef CAMERA_CONNECTOR_H
#define CAMERA_CONNECTOR_H


#include <iostream>
#include "opencv2/opencv.hpp"

#include <queue>
#include <thread>
#include <chrono>

#ifdef __arm__
#include <raspicam/raspicam_cv.h>
#endif

#define RASPBERRY_PI_CAM 1
#define USB_WEBCAM 2
#define IMAGE_FOLDER 3
#define USB_ID 500

// Location where images will be written to when write_images is called.
#define WRITE_LOCATION "/Users/adnankhan/Box Sync/Robots/1567_project/test_images/"

/**
Class makes a connection to an image source, this can be:

Built in Webcam/USB Webcam
Raspberry PI Cam (in case of raspberry pi)

Image folder (STILL WIP) that contains images
images will be loaded in order provided by folder and returned sequentially every
time get image is called
*/
class Camera_Connector {
private:

    cv::VideoCapture cam; ///< camera to get images from
    std::queue<cv::Mat> image_queue;
    std::queue<std::string> f_name_queue; ///< file names in folder (for hdd image source)
    std::string file_folder; ///< folder to look in for images
    int camera_source; /// <camera source based on dfinitions
#ifdef __arm__
    raspicam::RaspiCam_cv Camera;
    #endif

public:
    /**
    * Writes the matrix to a png file with the given filename
    */
    static void write_image(std::string filename, cv::Mat &img);

    void config_camera(int exp, int saturation, int interval);

    cv::Mat get_image();

    Camera_Connector() {
    }

    Camera_Connector(int camera_source, std::string source, int camera_id);

    ~Camera_Connector();
};


#endif