#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include <queue>
#include <thread>
#include <chrono>

#ifdef __arm__
#include <raspicam/raspicam_cv.h>
#endif

#define RASPBERRY_PI_CAM 1
#define USB_WEBCAM 2
#define IMAGE_FOLDER 3

/*
 Takes image from connected camera and
 */

class Camera_Connector {
    void set_camera_source();
    cv::VideoCapture cam;
    int step_detect(cv::Mat &source, int &intersection);
    std::queue<cv::Mat> image_queue;
    std::queue<std::string> f_name_queue;
    std::string file_folder;
    #ifdef __arm__
    raspicam::RaspiCam_cv Camera;
    #endif
    int camera_source;
    int get_image(cv::Mat & image_ret);

    int process_image(cv::Mat & rect_queue );
public:
    void test_image(int n);
    void config_camera(int exp, int saturation, int interval);
   // int get_image( char & buffer ,size_t buffer_size);
    Camera_Connector(int camera_source, std::string source);
    ~Camera_Connector();
};
