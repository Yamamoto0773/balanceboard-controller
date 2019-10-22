#include "BalanceBoard.hpp"

#include <iostream>


int main() {
    wii::BalanceBoard board;
    board.connect();

    board.on_led();

    if (board.serial_number() == wii::SerialNumber<char>("001f322938df")) {
        printf("OK\n");
    }

    printf("battery: %f\n", board.battery_percentage());

    _sleep(1000);

    while (1) {
        //_sleep(10);

        system("cls");
        board.update();

        if (board.front_button().is_down()) {
            std::cout << "pressed\n";
        }
    }
        
    return 0;
}
