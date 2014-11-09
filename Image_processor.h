#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H



#include "Camera_Connector.h"


#define CIRCLE_ATTEMPTS 3

using namespace cv;

class Image_Processor {
    int circle_detect(Camera_Connector &camera);

    int circle_detect(Mat &src);

    int rectangle_detect(Camera_Connector &camera);

    int rectangle_detect(Mat &src);

    bool check_within(Mat &mat, float bounding_percent, cv::Vector<cv::Point> contour);

public:

    void test_image(int n, int interval, Camera_Connector &camera);

    int step_detect(Camera_Connector &camera, int &intersection);
};

#endif