cmake_minimum_required(VERSION 2.8.4)
project(1567_lightdetect)
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Camera_Connector.cpp Camera_Connector.h Locator.h Locator.cpp Image_Processor.h Image_Processor.cpp Node.h Node.cpp Audio.h Audio.cpp Arduino_Connector.cpp Arduino_Connector.hpp Sennot_Graph.h Sennot_Graph.cpp Graph_Utils.hpp Graph_Utils.cpp)
set(TEST_FILES test.cpp Camera_Connector.cpp Camera_Connector.h Locator.h Locator.cpp Image_Processor.h Image_Processor.cpp Node.h Node.cpp Audio.h Audio.cpp Arduino_Connector.cpp Arduino_Connector.hpp Sennot_Graph.h Sennot_Graph.cpp Graph_Utils.hpp Graph_Utils.cpp)
add_executable(sennot_nav ${SOURCE_FILES})
add_executable(test_nav ${TEST_FILES})

target_link_libraries( sennot_nav ${OpenCV_LIBS} )
target_link_libraries( test_nav ${OpenCV_LIBS} )
