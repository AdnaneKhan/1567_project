cmake_minimum_required(VERSION 2.8.4)
project(1567_lightdetect)
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp camera_connector.cpp camera_connector.h)
add_executable(1567_lightdetect ${SOURCE_FILES})
target_link_libraries( 1567_lightdetect ${OpenCV_LIBS} )
