#include "BalanceBoardController.hpp"

BalanceBoardController::BalanceBoardController()
    : threshold_(1.0), last_update_time_(std::chrono::system_clock::now()) {}

BalanceBoardController::~BalanceBoardController() {
    off_led();
    disconnect();
}

bool BalanceBoardController::connect(const wii::SerialNumber<char>& serial_number, size_t find_limits) {
    for (size_t i = 0; i < find_limits; i++) {
        BalanceBoard::connect(i);
        if (BalanceBoard::serial_number_ == serial_number) {
            BalanceBoard::on_led();
            return true;
        } else {
            BalanceBoard::disconnect();
        }
    }

    return false;
}

bool BalanceBoardController::update() {
    tr_.update();
    tl_.update();
    br_.update();
    bl_.update();

    if (!BalanceBoard::update())
        return false;

    if (BalanceBoard::front_button().down())
        BalanceBoard::calibration();

    if (BalanceBoard::top_right() >= threshold_)
        tr_.press();
    else
        tr_.release();

    if (BalanceBoard::top_left() >= threshold_)
        tl_.press();
    else
        tl_.release();

    if (BalanceBoard::bottom_right() >= threshold_)
        br_.press();
    else
        br_.release();

    if (BalanceBoard::bottom_left() >= threshold_)
        bl_.press();
    else
        bl_.release();

    last_update_time_ = std::chrono::system_clock::now();

    return true;
}

double BalanceBoardController::time_from_last_update() {
    auto now = std::chrono::system_clock::now();       // 計測終了時刻を保存
    auto dur = now - last_update_time_;        // 要した時間を計算
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    return msec / 1000.0;
}
