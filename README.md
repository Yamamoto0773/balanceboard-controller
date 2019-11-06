# balanceboard-controller
Use Wii Balance Board as input device for miditone#, Koreisai 5J Project.

## Related Repositories
Koreisai2019 5J Project, miditone#, constructed by follwing repositories.
- Main Game   
  [miditone](https://github.com/shirry0129/miditone)
- Game Server  
  [miditone-server](https://github.com/Yamamoto0773/miditone-server)
- C++ HTTP Client for miditone-server  
  [miditone-client](https://github.com/Yamamoto0773/miditone-client)
- Use Wii Balance Board as input device  
  [balanceboard-controller (here)](https://github.com/Yamamoto0773/balanceboard-controller)

## Developed with
- Wii Remote access library  
[wiiuse](https://github.com/wiiuse/wiiuse)
- C++11

## Overview
- Async/Sync communication
- Connect Wii Balance Board with MAC Address (Serial Number)

## Requirements
- Support only Windows
- Need to link static library when build
- Need to link dynamic library when execute

## Usage
```cpp
#include "BalanceBoardController.hpp"

#include <iostream>
#include <chrono>
#include <unistd.h>

int main() {
    // Async version
    AsyncBalanceBoardController controller;

    // start connecting with serial number on async
    controller.start_connect("serial number", 5);

    while (1) {
        if (controller.is_connected()) {
            controller.stop_connect();

            std::cout << "battery:" << controller.battery_level() * 100 << "%\n";

            if (controller.start_update()) {
                std::cout << "update start\n";
            }

            controller.set_threshold(0.5);

            // print status of top left sensor of wii balance board on console.
            std::cout << std::fixed;
            std::cout << "tl :" << controller.top_left().pressed() << "\n";
            //std::cout << "tr :" << controller.top_right().down() << "\n";
            //std::cout << "bl :" << controller.bottom_left().down() << "\n";
            //std::cout << "br :" << controller.bottom_right().down() << "\n";
        } else {
            std::cout << "connecting...\n";

            sleep(3000);
        }
    }

    controller.disconnect();

    return 0;
}

```

## Library Changes
This software uses wiiuse library (https://github.com/wiiuse/wiiuse) and modified to implement features.
- Add method `wiiuse_serial_number` to get serial number via `struct wiimote_t`
- Add method argument `index` to `wiiuse_find` in order to specify the index of device to be enumerated first.


## License
This software is published under the **GNU General Public License v3.0** because of used library.  
Please read [License](/LICENSE) before use.
