#define BOOST_TEST_MODULE vk api
#define BOOST_TEST_DYN_LINK
#include <thread>
#include <boost/test/unit_test.hpp>
#include <vk-api/intervals-manager.h>

using vk_api::IntervalsManager;
using namespace std::chrono;

BOOST_AUTO_TEST_SUITE(vk_api_intervals)

BOOST_AUTO_TEST_CASE(interval, * boost::unit_test::timeout(2)) {
    IntervalsManager manager;

    auto begin = system_clock::now();
    manager.Wait(); // First wait should return immediately
    manager.Wait();
    auto finish = system_clock::now();
    auto time_passed = duration_cast<milliseconds>(finish - begin);

    BOOST_REQUIRE_CLOSE(time_passed.count(), 1.0 * IntervalsManager::kBaseInterval.count(), 5);
}

BOOST_AUTO_TEST_CASE(interval_increase, * boost::unit_test::timeout(2)) {
    IntervalsManager manager;

    auto begin = system_clock::now();
    manager.Wait(); // First wait should return immediately
    manager.IncreaseInterval();
    manager.Wait();
    auto finish = system_clock::now();
    auto time_passed = duration_cast<milliseconds>(finish - begin);

    BOOST_REQUIRE_CLOSE(time_passed.count(), (1.0 + 0.5) * IntervalsManager::kBaseInterval.count(), 5);
}

BOOST_AUTO_TEST_CASE(interval_increase3, * boost::unit_test::timeout(2)) {
    IntervalsManager manager;

    auto begin = system_clock::now();
    manager.Wait(); // First wait should return immediately
    manager.IncreaseInterval();
    manager.IncreaseInterval();
    manager.IncreaseInterval();
    manager.Wait();
    auto finish = system_clock::now();
    auto time_passed = duration_cast<milliseconds>(finish - begin);

    BOOST_REQUIRE_CLOSE(time_passed.count(), (1.0 + 3*0.5) * IntervalsManager::kBaseInterval.count(), 5);
}

BOOST_AUTO_TEST_CASE(interval_decrease, * boost::unit_test::timeout(4)) {
    IntervalsManager manager;

    manager.Wait(); // First wait should return immediately
    manager.IncreaseInterval();
    std::this_thread::sleep_for(IntervalsManager::kDecreaseInterval + milliseconds(100));
    auto begin = system_clock::now();
    manager.Wait(); // Should exit immediately
    auto finish = system_clock::now();
    auto time_passed = duration_cast<milliseconds>(finish - begin);

    BOOST_REQUIRE_SMALL((double)time_passed.count(), 100.);
}

BOOST_AUTO_TEST_SUITE_END()
