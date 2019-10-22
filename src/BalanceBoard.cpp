#include "BalanceBoard.hpp"

#include <chrono>
#include "../include/wiiuse.h"

#ifdef _DEBUG
#pragma comment(lib, "wiiuse_debug.lib")
#else
#pragma comment(lib, "wiiuse.lib")
#endif


namespace wii {
    Button::Button() : state_(State::Up) {}
    Button::~Button() {}

    bool Button::is_down() const {
        return state_ == State::Down || state_ == State::Pressed;
    }

    bool Button::is_up() const {
        return !is_down();
    }

    bool Button::is_pressed() const {
        return state_ == State::Pressed;
    }

    bool Button::is_released() const {
        return state_ == State::Released;
    }

    void Button::press() {
        state_ = State::Pressed;
    }

    void Button::release() {
        state_ = State::Released;
    }

    void Button::update() {
        if (state_ == State::Pressed)
            state_ = State::Down;
        else if (state_ == State::Released)
            state_ = State::Up;
    }


    void BasicDevice::deleter_t::cleanup(wiimote_t** wm, int n) {
        wiiuse_cleanup(wm, n);
    }

    BasicDevice::BasicDevice() : device_(wiiuse_init(1), deleter) {}

    BasicDevice::~BasicDevice() {}

    bool BasicDevice::is_connected() {
        return wiiuse_connect(device_.get(), 1);
    }

    void BasicDevice::disconnect() {
        wiiuse_disconnect(*device_);
        serial_number_.clear();
    }

    const SerialNumber<char>& BasicDevice::serial_number() const noexcept {
        return serial_number_;
    }

    BalanceBoard::BalanceBoard() :
        BasicDevice() {}

    BalanceBoard::~BalanceBoard() {}

    bool BalanceBoard::connect(int index) {
        wiiuse_find(device_.get(), 1, index, 0); // argument `timeout' is unused in library

        wiimote_t* wm = *device_;
        if (wm->exp.type != EXP_WII_BOARD) {
            wiiuse_disconnect(*device_);
        }

        wchar_t wstr[128];
        wiiuse_serial_number(*device_, wstr, sizeof(wstr));
        char str[256];
        wcstombs(str, wstr, 256);
        serial_number_.parse(str);

        return wiiuse_connect(device_.get(), 1);
    }

    void BalanceBoard::on_led() {
        wiiuse_set_leds(*device_, WIIMOTE_LED_1);
    }

    void BalanceBoard::off_led() {
        wiiuse_set_leds(*device_, WIIMOTE_LED_NONE);
    }

    void BalanceBoard::set_led(bool is_on) {
        is_on ? on_led() : off_led();
    }

    double BalanceBoard::battery_percentage() {
        return (*device_)->battery_level;
    }

    void BalanceBoard::update() {
        Button::update();

        if (!wiiuse_poll(device_.get(), 1))
            return ;

        wiimote_t* wm = *device_;
        if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_A)) {
            Button::press();
        } else if (IS_RELEASED(wm, WIIMOTE_BUTTON_A)) {
            Button::release();
        }

        printf("pressed  : %d\n", is_down());
        printf("released : %d\n", is_up());
        printf("just pr  : %d\n", is_pressed());
        printf("just re  : %d\n", is_released());

        if (wm->exp.type == EXP_WII_BOARD) {
            /* wii balance board */
            struct wii_board_t* wb = (wii_board_t*)&wm->exp.wb;
            float total = wb->tl + wb->tr + wb->bl + wb->br;
            float x = ((wb->tr + wb->br) / total) * 2 - 1;
            float y = ((wb->tl + wb->tr) / total) * 2 - 1;
            printf("Weight: %f kg @ (%f, %f)\n", total, x, y);
            printf("Interpolated weight: TL:%f  TR:%f  BL:%f  BR:%f\n", wb->tl, wb->tr, wb->bl, wb->br);
            printf("Raw: TL:%d  TR:%d  BL:%d  BR:%d\n", wb->rtl, wb->rtr, wb->rbl, wb->rbr);
        }
    }

}
