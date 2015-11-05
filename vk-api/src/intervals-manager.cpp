#include <vk-api/intervals-manager.h>
#include <thread>

namespace vk_api {
IntervalsManager::IntervalsManager(const std::chrono::milliseconds& interval)
        : current_interval_(interval) {
    last_time = std::chrono::steady_clock::now() - interval;
}

void IntervalsManager::Wait() {
    auto now = std::chrono::steady_clock::now();
    if (now > last_time + current_interval_) {
        return ;
    }
    auto diff = now - (last_time + current_interval_);
    std::this_thread::sleep_for(diff);
    last_time = now;
}

}
