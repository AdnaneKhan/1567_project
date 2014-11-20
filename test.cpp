#include "Arduino_Connector.hpp"
#include "Sennot_Graph.h"


void test_node() {
    // TEST NODE
    std::cout << "BEGIN TEST OF NODE:\n----------------------\n";

    Node * test = new Node('A');
    Node * test2 = new Node('B');
    Node * test3 = new Node('C');
    test->add_neighbor(test2, 5, DIR_N);
    test->add_neighbor(test3, 4, DIR_W);

    // Test node addition
    int expected_num = 2;
    if (test->num_neighbors() == expected_num) {
        std::cout << "We expected " << 2 << " neighbors and we had 2.\n\n";
    } else {
        std::cout << "There were " << test->num_neighbors() << " but we expected 2\n";
    }

    // Test node retrieval

    Node temp;
    test->get_neighbor((temp), DIR_N);
    std::cout << "The ID of the node was " << temp.node_label << std::endl;
    std::cout << " We expected B\n";



}

void test_graph() {
    std::cout << "BEGIN TEST OF GRAPH (LOCATOR):\n----------------------\n";
    // TEST GRAPH


    Sennot_Graph loc = Sennot_Graph();
    loc.graph_step();
    std::cout << loc.path_count() << std::endl;
    loc.graph_step();
    std::cout << loc.path_count() << std::endl;
    loc.graph_step();
    loc.graph_intersect(2);
    std::cout << loc.path_count() << std::endl;
    loc.graph_step();
    std::cout << loc.path_count() << std::endl;
    loc.graph_step();

    std::cout << loc.path_count() << std::endl;
    loc.graph_intersect(2);
    std::cout << loc.path_count() << std::endl;

}

void test_connector() {
    std::cout << "BEGIN TEST OF ARDUINO CONNECTOR:\n----------------------\n";

    // TEST ARDUINO CONNECTOR

    Arduino_Packet packet;
    Arduino_Connector con(&packet, "/dev/tty.usbmodem1451");

    con.init_connection();
    con.start_thread();

    std::chrono::milliseconds dura( 250 );

    for (int j = 0; j < 200; j ++) {
        for (int i = 0; i < 8; i++) {
            std::cout << packet.read(i) << ",";
        }
        std::cout << std::endl;

        std::this_thread::sleep_for(dura);
    }

    con.stop_thread();
    con.end_connection();
}

// TODO: circle detection test using image processor
void test_image_processor_circle() {
    std::cout << "BEGIN TEST OF IMAGE_PROCESSOR (CIRCLE, LIT):\n----------------------\n";
}

// TODO: rectangle detection test iamge processor
void test_image_processor_rectangle() {
    std::cout << "BEGIN TEST OF IMAGE_PROCESSOR (RECTANGLE, LIT):\n----------------------\n";
}

void camera_connector_test() {
    std::cout << "BEGIN TEST OF CAMERA_CONNECION:\n----------------------\n";
}

void audio_test() {
    std::cout << "BEGIN TEST OF AUDIO:\n----------------------\n";
}


int main() {

    //test_connector();
    test_node();
    test_graph();

    return 0;
}

