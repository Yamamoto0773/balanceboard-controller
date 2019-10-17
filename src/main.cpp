#include "BalanceBoard.hpp"

#include <iostream>

int main() {
    wii::BasicDevice device;

    std::cout << "start connection\n";

    while (!device.connect());
    std::cout << "connection success\n";

    device.update();

    std::cout << "Hello World!\n";
}
