#ifndef ELAPSEDTIMER_H
#define ELAPSEDTIMER_H

#include <chrono>

class ElapsedTimer
{
public:
    using Clock = std::chrono::steady_clock;

    explicit ElapsedTimer();
    explicit ElapsedTimer(const Clock::duration& timeout);

    bool timedOut() const;

    Clock::duration getElapsedTime() const;
    Clock::rep getElapsedMilliseconds() const;

    Clock::duration getRestOfTimeout() const;

private:
    Clock::time_point m_start;
    Clock::time_point m_end;
};

#endif // ELAPSEDTIMER_H
