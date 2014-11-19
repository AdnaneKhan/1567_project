#include "Arduino_Connector.hpp"

int main() {

    // TEST AUDIO CONNECTOR



    // TEST IMAGE PROCESSOR



    // TEST ARDUINO CONNECTOR
    Arduino_Packet packet;
    Arduino_Connector con(&packet, "/dev/tty.usbmodem1451");

    con.init_connection();
    con.start_thread();

    std::chrono::milliseconds dura( 250 );

    for (int j = 0; j < 200; j ++) {
        for (int i = 0; i < 8; i++) {
            std::cout << packet.read(i) << std::endl;
        }

         std::this_thread::sleep_for(dura);
    }

    con.stop_thread();
    con.end_connection();


    // TEST GRAPH



    // TEST NODE

    return 0;
}

