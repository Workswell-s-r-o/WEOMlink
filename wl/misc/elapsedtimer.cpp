#include "wl/misc/elapsedtimer.h"

#include <cassert>


namespace wl {

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
    return Clock::now() > m_end;
}

Clock::duration ElapsedTimer::getElapsedTime() const
{
    return Clock::now() - m_start;
}

Clock::rep ElapsedTimer::getElapsedMilliseconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(getElapsedTime()).count();
}

Clock::duration ElapsedTimer::getRestOfTimeout() const
{
    return m_end - Clock::now();
}

} // namespace wl
