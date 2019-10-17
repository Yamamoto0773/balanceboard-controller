#pragma once

#include <vector>
#include <memory>


struct wiimote_t;

namespace wii {
    class BasicDevice {
    public:
        BasicDevice();
        ~BasicDevice();

        bool connect();
        void disconnect();

        void update();

    private:
        struct deleter_t {
            void cleanup(wiimote_t**, int);

            void operator() (wiimote_t** wm) {
                cleanup(wm, 1);
            }
        } deleter;

        std::unique_ptr<wiimote_t*, deleter_t> device_;
    };


    class Bluetooth {
    public:
        static constexpr size_t max_devices = 4;

        Bluetooth();
        ~Bluetooth();

        void connect_devices(size_t numof_limits = 4, size_t timeout_sec = 5);
    private:
        std::vector<BasicDevice> devices_;
    };
}
