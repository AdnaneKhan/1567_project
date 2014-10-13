#include "camera_connector"

Camera_Connector::Camera_Connector(int camera_source, std::string source) {
    switch (camera_source){
        case USB_WEBCAM:

            break;
        case RASPBERRY_PI_CAM:
            break;
        case IMAGE_FOLDER:
            break;

    }
}