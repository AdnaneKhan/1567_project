#include "Image_Processor.h"

//#define DEBUG
/**
*  \param n number of times to take sample
*  \param interval time in ms to wait in between samples
*  \param camera connection with image source
*
*/
void Image_Processor::test_image(int n, int interval, Camera_Connector &camera) {
    std::chrono::milliseconds timespan(interval);

    int test;
    int hall_count = 0;
    int old_res = 0;
    int res = 0;

    for (int i = 0; i < n; i++) {
        old_res = res;
        res = this->step_detect(camera, test);
        if ((res ^ old_res) & res) {
            hall_count += 1;
            std::cout << "Lights: " << hall_count << std::endl;
        }

        std::this_thread::sleep_for(timespan);
    }

    std::cout << "The number of lights we passed: " << hall_count << std::endl;

    return;
}

/**
*  Checks whether the points of the counter are all located within a bounding rectangle that is
*  bounding_percent away from the edges of the matrix
*
*  \param mat matrix representing the image that is being checked
*  \param bounding_percent distance from the boundaries of the image that contour must exist within
*  \param countour vector of points that form contour to check
*/
bool Image_Processor::check_within(cv::Mat &mat, float bounding_percent, Vector<Point> contour) {
    bool retV = true;

    // Finding max and min x and y coords
    int y_min = mat.rows / 5;
    int y_max = mat.rows - y_min;

    int x_min = mat.cols / 5;
    int x_max = mat.cols - x_min;

    #ifdef DEBUG
         std::cout << "The y min is " << y_min << std::endl;
         std::cout << "The x min is " << x_min << std::endl;
    #endif

    for (cv::Vector<Point>::iterator point = contour.begin(); point != contour.end(); ++point) {
        // Shape contains points outside of the rectangle
    #ifdef DEBUG
        std::cout << "The y point is " << point->y << std::endl;
        std::cout << "The x point is " << point->x << std::endl;
    #endif
        if (point->x > x_max || point->x < x_max || point->y > y_max || point->y < y_min) {
            retV = false;
            break;
        }
    }

    #ifdef DEBUG
    std::cout << "The contour was " << retV << " in the mat.\n";
    #endif
    return retV;
}

/**
* \param camera to get get image from before checking circle
*/
detectionResult Image_Processor::circle_detect(Camera_Connector &camera) {

    Mat src = camera.get_image();

    return circle_detect(src);
}

/**
* Detects circle lights that are on
*/
detectionResult on_circle_detect(Mat & src) {
    detectionResult retV = 0;
    Mat dest;
    Mat thr;
    Mat canny;

    cvtColor(src, thr, CV_BGR2GRAY);
    GaussianBlur(thr, dest, Size(15, 15),2,2);
    threshold(dest,dest,230,255,CV_THRESH_BINARY);
    Canny(dest, canny, 100,200,3);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( canny, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);

    if (contours.size() > 0.0) {

        double area = contourArea(contours[0],false);
        float radius;
        Point2f center;
        minEnclosingCircle(contours[0], center, radius);

        #ifdef DEBUG
            imshow("img",src);
            waitKey(0);
            circle(src, center, radius,Scalar(255,0,0), 1,8,0);
            printf("%f area2\n",(radius*radius*3.14159265359));
            printf("%lf area\n",area);
            printf("div %f\n",(area/(radius*radius*3.14159265359) ));
        #endif

        if ( (area/(radius*radius*3.14159265359) ) > 0.90) {
            retV = 1;
        }
    }
    return retV;
}

/**
*  Detects circle lights that are off
*/
detectionResult off_circle_detect(Mat & src ){
    detectionResult retV = 0;
    Mat grey_src;

    cvtColor(src, grey_src, CV_BGR2GRAY);
    GaussianBlur(grey_src, grey_src, Size(7, 7),0,0);

    Canny(grey_src, grey_src, 50, 100, 3);

    vector<Vec3f> circles;
    HoughCircles(grey_src, circles, CV_HOUGH_GRADIENT, grey_src.rows / 3, 250, 100);


    return retV;
}

/**
* Referencing http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/hough_circle/hough_circle.html
* In creating this method.
*
* \param src matrix to check for circles
*/
detectionResult Image_Processor::circle_detect(Mat &src) {
    int retV = 0;

    retV = on_circle_detect(src);

   if (!retV) {
       retV = off_circle_detect(src);
   }

    return retV;
}

/**
*
*   \brief uses contour method to check for lit rectangles (lights) in image
*   \param src image matrix to check for rectangles
*   Uses a countour method to detect rectangles in image
*/
detectionResult Image_Processor::rectangle_detect(Mat &src) {
    int retV = 0;
    Mat thr;
    Mat blurred;
    Mat edges;

    cvtColor(src, thr, CV_BGR2GRAY);
    blurred = thr.clone();

    // Process image to isolate lights
    blur(thr, blurred, Size(25, 25));
    threshold(blurred, blurred, 225, 255, CV_THRESH_BINARY);

    Canny(blurred, edges, 20, 200, 3);

    #ifdef DEBUG
        Camera_Connector::write_image("rect_test", blurred);
    #endif

    vector<vector<Point> > contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
    int largest_contour_index = -1;

    // Init this to be large enough to represent a light
    int largest_area = 0;

    findContours(edges.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS); // Find the contours in the image

    for (int i = 0; i < contours.size(); i++) {
        if (contours[i].size() > 0 && check_within(src, .20, contours[i])) {
            double a = contourArea(contours[i], false);  //  Find the area of contour


            if (a > largest_area) {
                largest_area = a;
                largest_contour_index = i;                //Store the index of largest contour
            }
        }
    }

    // If we have a circular shape vs rectangle we are at an intersection
    if (contours.size() > 0) {
        retV = 1;
    }

    src = thr;
    return retV;
}

/**
*  \brief method to detect rectangles but first getting image from camera
*  \param camera to get image from prior to passing it to detector
*  \return whether rectangle was detected or not
*/
detectionResult Image_Processor::rectangle_detect(Camera_Connector &camera) {
    Mat src = camera.get_image();

    return rectangle_detect(src);
}

/**
* \param intersection set to 1 if intersection is detecteed
* \param camera to set image from
*/
detectionResult Image_Processor::step_detect(Camera_Connector &camera, int &intersection) {
    Mat src = camera.get_image();
    int step = rectangle_detect(src);
    src = camera.get_image();
    int circle = circle_detect(src);

    // If circle is detected intersection flag is set to 1
    if (circle) {
        intersection = 1;
    } else {
        intersection = 0;
    }

    return step;
}
