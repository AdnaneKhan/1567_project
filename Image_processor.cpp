#include "Image_processor.h"

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
    bool retV = false;

    // Finding max and min x and y coords
    int y_min = mat.rows / 5;
    int y_max = mat.rows - y_min;

    int x_min = mat.cols / 5;
    int x_max = mat.cols - x_min;

    for (cv::Vector<Point>::iterator point = contour.begin(); point != contour.end(); ++point) {
        // Shape contains points outside of the rectangle
        if (point->x > x_max || point->x < x_max || point->y > y_max || point->y < y_min) {
            retV = true;
            break;
        }
    }

    return retV;
}

/**
* \param camera to get get image from before checking circle
*/
int Image_Processor::circle_detect(Camera_Connector &camera) {

    Mat src = camera.get_image();

    return circle_detect(src);
}

/**
* Referencing http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/hough_circle/hough_circle.html
* In creating this method.
*
* \param src matrix to check for circles
*/
int Image_Processor::circle_detect(Mat &src) {
    int retV = 0;

    // Two detection possibilities for lights which are on/off
    Mat grey_src_on;
    Mat grey_src_off;

    Mat working_src = src;

    cvtColor(src, grey_src_on, CV_BGR2GRAY);
    cvtColor(src, grey_src_off, CV_BGR2GRAY);


    GaussianBlur(grey_src_off, grey_src_off, Size(15, 15), 2, 2);
    GaussianBlur(grey_src_on, grey_src_on, Size(15, 15), 2, 2);

    grey_src_on = grey_src_on + Scalar(-75, -75, -75);

    Camera_Connector::write_image("circles_grey_on", grey_src_on);
    Camera_Connector::write_image("circles_grey_off", grey_src_off);

    vector<Vec3f> circles;

    HoughCircles(grey_src_off, circles, CV_HOUGH_GRADIENT, 1, grey_src_off.rows / 8, 30, 50, 0, 0);

    /// Work on some radius filtering to make sure sprinklers do not get picked up
    for (int i = 0; i < CIRCLE_ATTEMPTS; i++) {
        if (circles.size() > 0) {

            retV = 1;
            break;
        }
        grey_src_on = grey_src_on + Scalar(-75, -75, -75);
        HoughCircles(grey_src_on, circles, CV_HOUGH_GRADIENT, 1, grey_src_on.rows / 8, 30, 50, 0, 0);
    }

    /// Draw the circles detected
    for (size_t i = 0; i < circles.size(); i++) {
        retV = 1;
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle(working_src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        // circle outline
        circle(working_src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
    }
    Camera_Connector::write_image("circles", grey_src_off);

    return retV;
}

/**
* \brief checks for rectangles using hough line intersection WIP
* \param src matrix to check hough rectangle
*
*  Referring to openCV docs for hough line detection
*  Still WIP, attmepting to determine presence of rectangles using intersections of hough lines.
*/
int Image_Processor::hough_rectangle_detect(Mat &src) {
    Mat working_src = src;

    Mat canny;
    Mat dst2;


    Canny(working_src, canny, 50, 200, 3);
    cvtColor(canny, dst2, CV_GRAY2BGR);
    Camera_Connector::write_image("grey_test", canny);

    vector<Vec2f> lines;

    HoughLines(canny, lines, 1, CV_PI / 180, 200, 0, 0);

    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(dst2, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
    }

//    vector<Vec4i> lines;
//    HoughLinesP(canny, lines, 1, CV_PI/180, 50, 100, 10 );
//    for( size_t i = 0; i < lines.size(); i++ )
//    {
//        Vec4i l = lines[i];
//        line( dst2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
//    }
//
    Camera_Connector::write_image("lines", dst2);

    return 0;
}

/**
*
*   \brief uses contour method to check for lit rectangles (lights) in image
*   \param src image matrix to check for rectangles
*   Uses a countour method to detect rectangles in image
*/
int Image_Processor::rectangle_detect(Mat &src) {
    int retV = 0;
    Mat thr;

    cvtColor(src, thr, CV_BGR2GRAY);


    // Process image to isolate lights
    threshold(thr, thr, 220, 255, CV_THRESH_BINARY);
    blur(thr, thr, Size(25, 25));
    addWeighted(thr, 8.0f, thr, 0.0f, 0.0f, thr, -1);
    vector<vector<Point> > contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
    int largest_contour_index = 0;
    int largest_area = 0;


    Mat dst(src.rows, src.cols, CV_8UC1, Scalar::all(0)); //create destination image
    findContours(thr.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS); // Find the contours in the image
    //std::cout << contours.size() << std::endl;

    for (int i = 0; i < contours.size(); i++) {

        if (contours[i].size() > 0) {
            double a = contourArea(contours[i], false);  //  Find the area of contour


            if (a > largest_area) {
                largest_area = a;
                largest_contour_index = i;                //Store the index of largest contour
            }
        }
    }


    // Need to detect whether the edges reach edge of screen, becuase we dont want to detect
    // off camera rectangles



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
int Image_Processor::rectangle_detect(Camera_Connector &camera) {
    Mat src = camera.get_image();

    return rectangle_detect(src);
}

/**
* \param intersection set to 1 if intersection is detecteed
* \param camera to set image from
*/
int Image_Processor::step_detect(Camera_Connector &camera, int &intersection) {
    Mat src = camera.get_image();
    int step = rectangle_detect(src);
    src = camera.get_image();
    int circle = circle_detect(src);

    if (circle) {
        intersection = 1;
    }

    return step;
}
