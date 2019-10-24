#include "BalanceBoardController.hpp"

#include <iostream>
#include <chrono>

int main() {
    BalanceBoardController controller;

    std::cout << "connecting...\n";

    while (!controller.is_connected()) {
        controller.connect("001f322938df");
        //_sleep(100);
    }

    // controller.set_threshold(0.5);

    std::cout << "connected!\n";
    std::cout << "battery:" << controller.battery_percentage() * 100 << "%\n";

    // _sleep(1000);

    while (1) {
        _sleep(10);
        system("cls");


        std::cout << "lag:" << controller.time_from_last_update() << "\n";

        controller.update();


        std::cout << std::fixed;
        std::cout << "tl :" << controller.top_left().down() << "\n";
        std::cout << "tr :" << controller.top_right().down() << "\n";
        std::cout << "bl :" << controller.bottom_left().down() << "\n";
        std::cout << "br :" << controller.bottom_right().down() << "\n";

        const auto& device = controller.device();

        std::cout << std::fixed;
        std::cout << "tl :" << device.top_left() << "\n";
        std::cout << "tr :" << device.top_right() << "\n";
        std::cout << "bl :" << device.bottom_left() << "\n";
        std::cout << "br :" << device.bottom_right() << "\n";
        
    }

    controller.disconnect();
        
    return 0;
}
