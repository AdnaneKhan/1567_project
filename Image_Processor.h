#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H



#include "Camera_Connector.h"



#define CIRCLE_ATTEMPTS 3

using namespace cv;

typedef int detectionResult;

class Image_Processor {

    bool check_within(Mat &mat, float bounding_percent, cv::Vector<cv::Point> contour);

public:

    detectionResult circle_detect(Camera_Connector &camera);

    detectionResult circle_detect(Mat &src);

    detectionResult rectangle_detect(Camera_Connector &camera);

    detectionResult rectangle_detect(Mat &src);

    void test_image(int n, int interval, Camera_Connector &camera);

    detectionResult step_detect(Camera_Connector &camera, int &intersection);
};

#endif