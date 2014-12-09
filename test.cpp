#define DEBUG
#define LOGGING

#include "Arduino_Connector.hpp"
#include "Sennot_Graph.h"
#include "Audio.h"
#include "Camera_Connector.h"
#include "Image_Processor.h"
#include "Graph_Utils.hpp"
//

#define MOCK_IMAGE_FOLDER "harvested_images/"

//#include <list>
//
//void test_node() {
//    // TEST NODE
//    std::cout << "BEGIN TEST OF NODE:\n----------------------\n";
//
//    Node * test = new Node('A');
//    Node * test2 = new Node('B');
//    Node * test3 = new Node('C');
//    test->add_neighbor(test2, 5, DIR_N);
//    test->add_neighbor(test3, 4, DIR_W);
//
//    // Test node addition
//    int expected_num = 2;
//    if (test->num_neighbors() == expected_num) {
//        std::cout << "We expected " << 2 << " neighbors and we had 2.\n\n";
//    } else {
//        std::cout << "There were " << test->num_neighbors() << " but we expected 2\n";
//    }
//
//    // Test node retrieval
//
//    Node temp;
//    test->get_neighbor((temp), DIR_N);
//    std::cout << "The ID of the node was " << temp.node_id << std::endl;
//    std::cout << " We expected B\n";
//
//}
//
//void test_graph() {
//    std::cout << "BEGIN TEST OF GRAPH (LOCATOR):\n----------------------\n";
//    // TEST GRAPH
//
//
//    Sennot_Graph loc = Sennot_Graph();
//    loc.edge_step();
//    std::cout << loc.path_count() << std::endl;
//    loc.edge_step();
//    std::cout << loc.path_count() << std::endl;
//    loc.edge_step();
//    loc.intersection_update(2);
//    std::cout << loc.path_count() << std::endl;
//    loc.edge_step();
//    std::cout << loc.path_count() << std::endl;
//    loc.edge_step();
//
//    std::cout << loc.path_count() << std::endl;
//    loc.intersection_update(2);
//    std::cout << loc.path_count() << std::endl;
//
//}
//
//void graph_search_test() {
//    std::cout << "BEGIN TEST OF GRAPH (GOAL SEARCH):\n----------------------\n";
//
//
//    // Set of node labels to serve as source
//    Sennot_Graph loc = Sennot_Graph();
//
//
//
//     std::vector<nodeLabel> result = loc.find_path('A','I');
//
//
//    for (graphInt n : result) {
//        std::cout << (char)n << "->";
//    }
//    std::cout << std::endl;
//
//     result = loc.find_path('A','M');
//
//
//    for (graphInt n : result) {
//        std::cout << (char)n << "->";
//    }
//    std::cout << std::endl;
//    // Destination
//
//
//    result = loc.find_path('S','E');
//
//
//    for (graphInt n : result) {
//        std::cout << (char)n << "->";
//    }
//    std::cout << std::endl;
//}
//
//void test_connector() {
//    std::cout << "BEGIN TEST OF ARDUINO CONNECTOR:\n----------------------\n";
//
//    // TEST ARDUINO CONNECTOR
//
//    Arduino_Packet packet;
//    Arduino_Connector con(&packet, "/dev/tty.usbmodem1451", ARDUINO);
//
//    con.init_connection();
//    con.start_thread();
//
//    std::chrono::milliseconds dura( 250 );
//
//    for (int j = 0; j < 200; j ++) {
//        for (int i = 0; i < 8; i++) {
//            std::cout << packet.read(i) << ",";
//        }
//        std::cout << std::endl;
//
//        std::this_thread::sleep_for(dura);
//    }
//
//    con.stop_thread();
//    con.end_connection();
//}
//
//// TODO: circle detection test using image processor
//void test_image_processor_circle() {
//    std::cout << "BEGIN TEST OF IMAGE_PROCESSOR (CIRCLE, LIT):\n----------------------\n";
//}
//
//// TODO: rectangle detection test iamge processor
//void test_image_processor_rectangle() {
//    std::cout << "BEGIN TEST OF IMAGE_PROCESSOR (RECTANGLE, LIT):\n----------------------\n";
//}
//
//void camera_connector_test() {
//    std::cout << "BEGIN TEST OF CAMERA_CONNECION:\n----------------------\n";
//
//    #ifdef __arm__
//        Camera_Connector camera(Camera_Type::RASPBERRY_PI_CAM_E,"",0);
//    #else
//        Camera_Connector camera(Camera_Type ::USB_WEBCAMS_E,"",0);
//    #endif
//
//    std::chrono::milliseconds timespan(40000);
//    std::chrono::milliseconds shortspan(1000);
//    std::this_thread::sleep_for(timespan);
//
//    for (int i = 0; i < 20; i++) {
//        cv::Mat cam_img = camera.get_image();
//        std::cout << "Wrote image: " << i << std::endl;
//        camera.write_image("test" + std::to_string(i), cam_img);
//        std::chrono::milliseconds shortspan(1000);
//    }
//
//    camera.close_camera();
//}
//
//void audio_test() {
//    std::cout << "BEGIN TEST OF AUDIO:\n----------------------\n";
//
//    Audio::play_left();
//    Audio::intersection();
//    Audio::play_light();
//    Audio::play_right();
//    Audio::play_forward();
//
//    std::cout << "Now testing direction selector:\n";
//    Audio::turn_dir(0);
//    Audio::turn_dir(2);
//}
//
//void image_processor_test() {
//    std::cout << "BEGIN TEST OF IMAGE PROCESSOR:\n----------------------\n";
//
//    Camera_Connector con (Camera_Type::IMAGE_FOLDER_E, MOCK_IMAGE_FOLDER, 190);
//    Image_Processor cv_processor = Image_Processor();
//
//
//
//    for (int i = 0; i < 190; i++) {
//        cv::Mat img = con.get_image();
//        int res = cv_processor.circle_detect(img);
//        int light_res = cv_processor.rectangle_detect(img);
//
//        std::cout << " For Image " << i << ":";
//        std::cout << " The light was " << light_res << " and the circle was " << res << std::endl;
//
//    }
//
//}
//
//void live_processor_test() {
//    std::cout << "BEGIN TEST OF LIVE IMAGE PROCESSOR:\n----------------------\n";
//
//
//#ifdef __arm__
//        Camera_Connector con(Camera_Type ::RASPBERRY_PI_CAM_E,"",0);
//    #else
//    Camera_Connector con(Camera_Type ::USB_WEBCAMS_E,"",0);
//#endif
//
//    Image_Processor cv_processor = Image_Processor();
//
//
//    for (int i = 0; i < 200; i++) {
//        cv::Mat img = con.get_image();
//        int res = cv_processor.circle_detect(img);
//        int light_res = cv_processor.rectangle_detect(img);
//
//        if (res) {
//            Audio::intersection();
//        } else if (light_res) {
//            Audio::play_light();
//        }
//
//        std::cout << " For Image " << i << ":";
//        std::cout << " The light was " << light_res << " and the circle was " << res << std::endl;
//
//    }
//}
//
//void graph_direction_test() {
//    std::cout << "BEGIN TEST OF GRAPH_UTILS:\n----------------------\n";
//
//    // SOUTH
//    int turn = DIR_S;
//
//    // TURNING EAST
//
//    int heading = DIR_E;
//
//    int res = Graph_Utils::cardinal_to_hand(turn, heading);
//
//    if (res != LEFT) {
//        std::cout <<"Problem with dir convertor!\n";
//        std::cout <<"We excpected " << LEFT << " but got back " << res << std::endl;
//    }
//
//
//    // WEST
//    turn = DIR_W;
//    heading = DIR_N;
//
//    res = Graph_Utils::cardinal_to_hand(turn, heading);
//    if (res != RIGHT) {
//        std::cout <<"Problem with dir convertor!\n";
//        std::cout <<"We excpected " << LEFT << " but got back " << res << std::endl;
//    }
//
//
//    turn = LEFT;
//    heading = DIR_N;
//
//    res = Graph_Utils::hand_to_cardinal(turn, heading);
//    if (res != DIR_W) {
//        std::cout <<"Problem with hand dir convertor!\n";
//        std::cout <<"We excpected " << DIR_W << " but got back " << res << std::endl;
//    }
//
//
//    turn = RIGHT;
//    heading = DIR_S;
//
//    res = Graph_Utils::hand_to_cardinal(turn, heading);
//    if (res != DIR_W) {
//        std::cout <<"Problem with hand dir convertor!\n";
//        std::cout <<"We excpected " << DIR_W << " but got back " << res << std::endl;
//    }
//
//
//    turn = FORWARD;
//    heading = DIR_E;
//
//    res = Graph_Utils::hand_to_cardinal(turn, heading);
//    if (res != DIR_E) {
//        std::cout <<"Problem with hand dir convertor!\n";
//        std::cout <<"We excpected " << DIR_E << " but got back " << res << std::endl;
//    }
//
//
//}
//
int main() {
//
////    //test_connector();
////    test_node();
////    test_graph();
////    audio_test();
////    //camera_connector_test();
////    graph_direction_test();
////    graph_search_test();
////    //image_processor_test();
////    //live_processor_test();

    return 0;
}
//
