#include "elapsedtimer.h"

#include <cassert>


ElapsedTimer::ElapsedTimer() :
    ElapsedTimer(std::chrono::milliseconds(0))
{
}

ElapsedTimer::ElapsedTimer(const Clock::duration& timeout)
{
    m_start = Clock::now();
    m_end = m_start + timeout;
}

bool ElapsedTimer::timedOut() const
{
    return std::chrono::steady_clock::now() > m_end;
}

ElapsedTimer::Clock::duration ElapsedTimer::getElapsedTime() const
{
    return std::chrono::steady_clock::now() - m_start;
}

ElapsedTimer::Clock::rep ElapsedTimer::getElapsedMilliseconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(getElapsedTime()).count();
}

ElapsedTimer::Clock::duration ElapsedTimer::getRestOfTimeout() const
{
    return m_end - std::chrono::steady_clock::now();
}
