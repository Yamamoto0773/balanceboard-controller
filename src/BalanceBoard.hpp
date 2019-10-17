#pragma once

#include <vector>
#include <memory>


struct wiimote_t;

namespace wii {
    class Button {
    public:
        enum State {
            Pressed,    // 押された瞬間
            Released,   // 離された瞬間
            Down,       // 押されている状態
            Up,         // 離されている状態
        };

        Button();
        ~Button();

        bool is_down() const;
        bool is_up() const;
        bool is_pressed() const;
        bool is_released() const;

        const Button& self() const noexcept { return *this; };

    protected:
        void press();
        void release();
        void update();

    private:
        State state_;
    };


    class BasicDevice {
    public:
        BasicDevice();
        ~BasicDevice();

        bool is_connected();
        void disconnect();
        virtual bool connect() = 0;

    protected:
        struct deleter_t {
            void cleanup(wiimote_t**, int);

            void operator() (wiimote_t** wm) {
                cleanup(wm, 1);
            }
        } deleter;

        std::unique_ptr<wiimote_t*, deleter_t> device_;
    };


    class BalanceBoard : protected Button, public BasicDevice {
    public:
        BalanceBoard();
        ~BalanceBoard();

        bool connect() override;

        void on_led();
        void off_led();
        void set_led(bool is_on);

        const Button& front_button() const noexcept { return Button::self(); }

        void update();

    private:
        Button front_button_;
    };
}
