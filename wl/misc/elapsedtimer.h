#ifndef WL_ELAPSEDTIMER_H
#define WL_ELAPSEDTIMER_H

#include "wl/time.h"

namespace wl {

/**
 * @class ElapsedTimer
 * @headerfile elapsedtimer.h "wl/misc/elapsedtimer.h"
 * @brief A timer class for measuring elapsed time and handling timeouts.
 */
class ElapsedTimer
{
public:
    /**
     * @brief Constructs an ElapsedTimer without a timeout.
     */
    explicit ElapsedTimer();

    /**
     * @brief Constructs an ElapsedTimer with a specified timeout duration.
     * @param timeout The timeout duration for the timer.
     */
    explicit ElapsedTimer(const Clock::duration& timeout);

    /**
     * @brief Checks if the specified timeout duration has elapsed.
     * @return True if the timer has timed out, false otherwise.
     */
    bool timedOut() const;

    /**
     * @brief Retrieves the elapsed time since the timer started.
     * @return The duration since the timer's start.
     */
    Clock::duration getElapsedTime() const;

    /**
     * @brief Retrieves the elapsed time in milliseconds since the timer started.
     * @return The elapsed time in milliseconds as a Clock::rep value.
     */
    Clock::rep getElapsedMilliseconds() const;

    /**
     * @brief Calculates the remaining time until the timer times out.
     * @return The remaining duration until the timer reaches the timeout.
     */
    Clock::duration getRestOfTimeout() const;

private:
    Clock::time_point m_start;
    Clock::time_point m_end;
};

} // namespace wl

#endif // WL_ELAPSEDTIMER_H
