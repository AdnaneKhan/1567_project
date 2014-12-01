#ifndef CONSTANT_DEFS_H
#define CONSTANT_DEFS_H


typedef enum Camera_Type
{
    RASPBERRY_PI_CAM_E,
    USB_WEBCAMS_E,
    IMAGE_FOLDER_E
} CTypeEnum;


typedef enum Turn_type
{
    LEFT = 3,
    RIGHT = 1,
    FORWARD = 0,
    REVERSE= 2,
} TurnTypeEnum;

typedef enum Cardinal_Directions
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 4
} CardinalEnum;


#endif