#pragma once
#include <chrono>

namespace vk_api {

class IntervalsManager{
public:
    typedef std::chrono::steady_clock Clock;

    IntervalsManager();

    void Wait();
    void IncreaseInterval();

    static const std::chrono::milliseconds kBaseInterval;
    static const std::chrono::milliseconds kDecreaseInterval;

private:

    Clock::time_point last_request;
    Clock::time_point last_increase;

    unsigned increase_level_;

    std::chrono::milliseconds CalculateInterval(const Clock::time_point& now);
};

}
