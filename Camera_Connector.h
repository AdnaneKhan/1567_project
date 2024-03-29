#ifndef CAMERA_CONNECTOR_H
#define CAMERA_CONNECTOR_H


#include <iostream>
#include "opencv2/opencv.hpp"

#include <queue>
#include <thread>
#include <chrono>

#ifdef __arm__
#include <raspicam/raspicam.h>
#include <raspicam/raspicam_cv.h>
#endif

#include "ConstantDefs.hpp"


// Location where images will be written to when write_images is called.
#ifdef __arm__
#define WRITE_LOCATION "/home/pi/test_images/"
#else
#define WRITE_LOCATION "/Users/adnankhan/Box Sync/Robots/1567_project/test_images/"
#endif

/**
Class makes a connection to an image source, this can be:

Built in Webcam/USB Webcam
Raspberry PI Cam (in case of raspberry pi)

Image folder (STILL WIP) that contains images
images will be loaded in order provided by folder and returned sequentially every
time get image is called
*/

class Camera_Connector
{

private:

    std::queue<std::string> f_name_queue; ///< file names in folder (for hdd image source)
    std::string file_folder; ///< folder to look in for images

    CTypeEnum camera_source; /// <camera source based on dfinitions

#ifdef __arm__
        raspicam::RaspiCam_Cv Camera;
    #endif
    cv::VideoCapture cam; ///< camera to get images from

public:

    /*
    CLoses the connection with the camera
     */
    void close_camera();

    /**
    * Writes the matrix to a png file with the given filename
    */
    static void write_image(std::string filename, cv::Mat &img);


    /*
    Configure camera properties
     */
    void config_camera(int exp, int saturation, int interval);


    /*
    Gets a new image from the camera
     */
    cv::Mat get_image();

    Camera_Connector()
    {
    }

    /*
     Initialize a new camera connection
     */
    void config_connector(CTypeEnum camera_source, std::string source, int camera_number);

    ~Camera_Connector();
};


#endif