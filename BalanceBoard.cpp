#include "BalanceBoard.hpp"

#include "src/wiiuse/wiiuse.h"


namespace wii {
    BalanceBoard::BalanceBoard(lib::wiimote* device_ptr) {
        device_.reset(device_ptr);
    }

    BalanceBoard::~BalanceBoard() {}
    


    namespace Bluetooth {
        std::vector<BalanceBoard> find_device(size_t numof_limits, size_t timeout_sec) {
            wiimote** wiimotes = wiiuse_init(numof_limits);

            
        }
    }
}
