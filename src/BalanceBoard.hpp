#pragma once

#include <vector>
#include <memory>
#include <string>
#include <array>


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


    template <typename char_type>
    class SerialNumber {
    public:
        using string_type = std::basic_string<char_type>;

        SerialNumber() {}
        SerialNumber(string_type serial_number_str) {
            parse(serial_number_str);
        }

        ~SerialNumber() {};

        void parse(const string_type& serial_number_str) {
            for (size_t i = 0; i < 6; i++) {
                auto str = serial_number_str.substr(i * 2, 2);
                std::transform(
                    str.begin(),
                    str.end(),
                    str.begin(),
                    [](char_type ch) { return std::tolower(ch); }
                );
                sn_[5 - i] = str;
            }
        }

        void clear() {
            for (auto& s : sn_) {
                s.clear();
            }
        }

        std::array<string_type, 6> sn_;
    };

    template <typename char_type>
    bool operator ==(const SerialNumber<char_type>& a, const SerialNumber<char_type>& b) {
        for (size_t i = 0; i < 6; i++) {
            if (a.sn_.at(i) != b.sn_.at(i))
                return false;
        }

        return true;
    }


    class BasicDevice {
    public:
        BasicDevice();
        ~BasicDevice();

        bool is_connected();
        void disconnect();
        virtual bool connect(int index = 0) = 0;

        const SerialNumber<char>& serial_number() const noexcept;

    protected:
        struct deleter_t {
            void cleanup(wiimote_t**, int);

            void operator() (wiimote_t** wm) {
                cleanup(wm, 1);
            }
        } deleter;

        std::unique_ptr<wiimote_t*, deleter_t> device_;
        SerialNumber<char> serial_number_;
    };


    class BalanceBoard : protected Button, public BasicDevice {
    public:
        BalanceBoard();
        ~BalanceBoard();

        bool connect(int index = 0) override;

        void on_led();
        void off_led();
        void set_led(bool is_on);

        // between 0.0 and 1.0f
        double battery_percentage();

        const Button& front_button() const noexcept { return Button::self(); }

        float top_left() const noexcept;
        float top_right() const noexcept;
        float bottom_left() const noexcept;
        float bottom_right() const noexcept;

        void update();

    private:
        Button front_button_;
    };
}
