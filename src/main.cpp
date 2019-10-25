#include "BalanceBoardController.hpp"

#include <iostream>
#include <chrono>

int main() {
    AsyncBalanceBoardController controller;

    controller.start_connect("001f322938df", 5);
    
    //controller.wait_connect();

    while (1) {
        if (controller.is_connected()) {
            controller.stop_connect();

            std::cout << "battery:" << controller.battery_level() * 100 << "%\n";

            if (controller.start_update()) {
                std::cout << "update start\n";
            }

            controller.set_threshold(0.5);

            //_sleep(10);
            //system("cls");

            //std::cout << "lag:" << controller.time_from_last_update() << "\n";

            std::cout << std::fixed;
            std::cout << "tl :" << controller.top_left().pressed() << "\n";
            //std::cout << "tr :" << controller.top_right().down() << "\n";
            //std::cout << "bl :" << controller.bottom_left().down() << "\n";
            //std::cout << "br :" << controller.bottom_right().down() << "\n";
        } else {
            std::cout << "connecting...\n";
          

            _sleep(3000);
        }

    }

    controller.disconnect();
        
    return 0;
}
