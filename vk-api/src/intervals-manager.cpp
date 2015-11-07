#include <vk-api/intervals-manager.h>
#include <thread>
#include <iostream>

namespace vk_api {

const std::chrono::milliseconds IntervalsManager::kBaseInterval(400);
const std::chrono::milliseconds IntervalsManager::kDecreaseInterval(3000);

IntervalsManager::IntervalsManager() : increase_level_(0) {
    last_request = Clock::now() - kBaseInterval;
}

void IntervalsManager::Wait() {
    auto now = Clock::now();
    auto interval = CalculateInterval(now);

    auto awake_time = last_request + interval;

    std::chrono::milliseconds tolerance(50);
    if (awake_time <= now + tolerance) {
        last_request = Clock::now();
        return ;
    }

    std::this_thread::sleep_until(awake_time);
    last_request = Clock::now();
}

void IntervalsManager::IncreaseInterval() {
    ++increase_level_;
    last_increase = Clock::now();
}

std::chrono::milliseconds IntervalsManager::CalculateInterval(const Clock::time_point& now) {
    if (increase_level_ != 0 && (now - last_increase > kDecreaseInterval)) {
        --increase_level_;
    }
    auto interval = kBaseInterval + 0.5 * kBaseInterval * increase_level_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(interval);
}

}
