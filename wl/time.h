#ifndef WL_TIME_H
#define WL_TIME_H

#include <chrono>
#include <functional>

namespace wl {

/**
 * @class Clock
 * @brief Alias for the steady clock used to measure time intervals.
 */
using Clock = std::chrono::steady_clock;

/**
 * @class SleepFunction
 * @brief Function type for implementing custom sleep behavior.
 * 
 * `SleepFunction` is a callable that takes a duration (`Clock::duration`) as input 
 * and performs the corresponding delay. This allows customization of sleep functionality, 
 * enabling platform-specific or mocked implementations for different environments.
 */
using SleepFunction = std::function<void(const Clock::duration&)>;

} // namespace wl

#endif // WL_TIME_H