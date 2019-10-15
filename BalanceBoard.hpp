#pragma once

#include <vector>
#include <memory>


namespace wii {
    namespace lib {
        class wiimote;
    }
    
    class BalanceBoard {
    public:
        BalanceBoard(lib::wiimote* device_ptr);
        ~BalanceBoard();



    private:
        std::unique_ptr<lib::wiimote> device_;
    };


    class Bluetooth {
    public:
        static constexpr size_t max_divices = 4;

        Bluetooth();
        ~Bluetooth();

        const std::vector<BalanceBoard>& find_devices(size_t numof_limits = 4, size_t timeout_sec = 5);
    private:
        std::unique_ptr<BalanceBoard> divices_;
    };
}
