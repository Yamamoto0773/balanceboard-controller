#include "BalanceBoardController.hpp"

#include <thread>

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
            BalanceBoardController::calibration();
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


bool BalanceBoardController::calibration() {
    if (!update())
        return false;

    BalanceBoard::calibration();

    return true;
}



AsyncBalanceBoardController::AsyncBalanceBoardController() {}

AsyncBalanceBoardController::~AsyncBalanceBoardController() {
    if (thread_.first.joinable())
        thread_.first.detach();

    board_.disconnect();
}

bool AsyncBalanceBoardController::start_connect(const wii::SerialNumber<char>& serial_number, size_t find_limits) {
    if (thread_.first.joinable())
        return false; // thread already exists

    auto connect_async_func = [this, &serial_number, find_limits] () {
        while (!board_.is_connected()) {
            board_.connect(serial_number, find_limits);
        }

        return ;
    };

    thread_.first = std::thread(connect_async_func);
    thread_.second = async_operation_type::connect;

    return true;
}

bool AsyncBalanceBoardController::wait_connect() {
    if (thread_.second != async_operation_type::connect)
        return false;

    if (thread_.first.joinable())
        thread_.first.join();

    return true;
}

bool AsyncBalanceBoardController::stop_connect() {
    if (thread_.second != async_operation_type::connect)
        return false;

    if (thread_.first.joinable())
        thread_.first.detach();

    return true;
}

bool AsyncBalanceBoardController::start_update() {
    if (thread_.first.joinable())
        return false; // thread already exists

    auto update_async_func = [this] () {
        while (1)
            board_.update();
    };

    thread_.first = std::thread(update_async_func);
    thread_.second = async_operation_type::update;

    return true;
}

bool AsyncBalanceBoardController::stop_update() {
    if (thread_.second != async_operation_type::update)
        return false;

    if (thread_.first.joinable())
        thread_.first.detach();

    return true;
}

void AsyncBalanceBoardController::set_threshold(float kg) {
    std::lock_guard<std::mutex> lock(board_mutex_);

    board_.set_threshold(kg);
}

void AsyncBalanceBoardController::disconnect() {
    if (thread_.first.joinable())
        thread_.first.detach();

    board_.disconnect();
}

float AsyncBalanceBoardController::battery_level() {
    std::lock_guard<std::mutex> lock(board_mutex_);

    return board_.battery_level();
}

bool AsyncBalanceBoardController::is_connected() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.is_connected();
}

wii::Button AsyncBalanceBoardController::top_left() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.top_left();
}

wii::Button AsyncBalanceBoardController::top_right() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.top_right();
}

wii::Button AsyncBalanceBoardController::bottom_left() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.bottom_left();
}

wii::Button AsyncBalanceBoardController::bottom_right() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.bottom_right();
}

double AsyncBalanceBoardController::time_from_last_update() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.time_from_last_update();
}

bool AsyncBalanceBoardController::calibration() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.calibration();
}
