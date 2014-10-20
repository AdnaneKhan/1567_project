#include "camera_connector.h"

using namespace cv;

class camera_exception: public std::exception {
    virtual const char* what() const throw() {
        return "The camera ID specified was not valid";
    }
} camera_ex;


void Camera_Connector::test_image(int n) {
    cv::Mat camera_test;
    std::chrono::milliseconds timespan(1000);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    int test;
    int hall_count= 0;
    int old_res = 0;
    int res = 0;
for (int i = 0; i < n; i++) {
    get_image(camera_test);
    old_res = res;
    res = step_detect(camera_test, test);
    hall_count += (res ^ old_res) & res;
    cv::imwrite("/Users/adnankhan/Box Sync/Robots/1567_project/test_images/"+std::to_string(i)+".png" ,camera_test,compression_params);
    std::this_thread::sleep_for(timespan);
}

    std::cout << "The number of lights we passed: " << hall_count << std::endl;

    return;
}

int Camera_Connector::process_image(cv::Mat & rect_queue ) {

    cv::Mat camera_cap;
    get_image(camera_cap);


    // Rectangle is now normalized to 2D view
    cv::Mat camera_result = camera_cap;

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
        if (f_name_queue.size() > 0) {
            std::string file_name = f_name_queue.front();
            f_name_queue.pop();

            cv::VideoCapture file_in(file_name);
            file_in.read(image_ret);
        } else {
            // No more files from source, throwing exception
            throw camera_ex;
        }

    } else if (camera_source == RASPBERRY_PI_CAM) {
    #ifdef __arm__
        Camera.retrieve(image_ret);
    #endif
    } else {
        cam.read(image_ret);
    }

    return 0;
}

/*

Citing https://github.com/bsdnoobz/opencv-code/blob/master/quad-segmentation.cpp

 */
int Camera_Connector::step_detect(cv::Mat &src, int &intersection) {
    cv::Mat thr;
    cv::Mat processed = src;
    int detected = 0;

    cvtColor(src, thr,CV_BGR2GRAY);


    // Process image to isolate lights
    threshold(thr, thr,220,255,CV_THRESH_BINARY );
    blur( thr,thr, Size( 25, 25 ) );
    addWeighted(thr,8.0f, thr,0.0f,0.0f,thr,-1);

    vector< vector <Point> > contours; // Vector for storing contour
    vector< Vec4i > hierarchy;
    int largest_contour_index=0;
    int largest_area=0;

    Mat dst(src.rows,src.cols,CV_8UC1,Scalar::all(0)); //create destination image
    findContours( thr.clone(), contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS ); // Find the contours in the image
    //std::cout << contours.size() << std::endl;

    for( int i = 0; i< contours.size(); i++ ){
        double a=contourArea( contours[i],false);  //  Find the area of contour

        if(a>largest_area){
            largest_area=a;
            largest_contour_index=i;                //Store the index of largest contour
        }
    }


    // Need to detect whether the edges reach edge of screen, becuase we dont want to detect
    // off camera rectangles


    // If we have a circular shape vs rectangle we are at an intersection
    if (contours.size() > 0) {
        detected = 1;
    }
//    for (int i =0; i < contours.size(); i++) {
//
//        drawContours( dst,contours, i, Scalar(255,255,255),CV_FILLED, 40, hierarchy );
//        vector<vector<Point> > contours_poly(1);
//        approxPolyDP( Mat(contours[largest_contour_index]), contours_poly[0],5, true );
//
//
//    }
    src = thr;
    return detected;
}

int usb_camera_init(cv::VideoCapture & to_init ) {

        if (to_init.open(500)){
            std::cout << "Found Camera " << 500 << '\n';
            std::chrono::milliseconds timespan(500);
            std::this_thread::sleep_for(timespan);
            //break;
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

            // Read names of all images in folder
            // push all names to queue

        Camera_Connector::camera_source = camera_source;
            break;
        default:
            throw camera_ex;
    }

}

Camera_Connector::~Camera_Connector() {
    cam.~VideoCapture();

}