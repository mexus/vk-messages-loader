#pragma once
#include <chrono>

namespace vk_api {

class IntervalsManager{
public:
    IntervalsManager(const std::chrono::milliseconds& interval = std::chrono::milliseconds(400));

    void Wait();

private:
    std::chrono::milliseconds current_interval_;
    std::chrono::steady_clock::time_point last_time;
};

}
