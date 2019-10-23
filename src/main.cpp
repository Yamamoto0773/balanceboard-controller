#include "BalanceBoard.hpp"

#include <iostream>
#include <chrono>

int main() {
    wii::BalanceBoard board;

    std::cout << "connecting...\n";
    

    board.connect();
    if (board.serial_number() == wii::SerialNumber<char>("001f322938df")) {
        printf("OK\n");
    } else {
        return false;
    }
    


    board.on_led();
    printf("battery: %f\n", board.battery_percentage());

    _sleep(1000);

    while (1) {
        _sleep(10);

        system("cls");

        board.update();

        if (board.is_connected())
            std::cout << "connect\n";

        if (board.front_button().is_pressed()) {
            board.calibration();
        }

        std::cout << std::fixed;
        std::cout << "tl :" << board.top_left() << "\n";
        std::cout << "tr :" << board.top_right() << "\n";
        std::cout << "bl :" << board.bottom_left() << "\n";
        std::cout << "br :" << board.bottom_right() << "\n";

    }

    board.off_led();
    board.disconnect();
        
    return 0;
}
