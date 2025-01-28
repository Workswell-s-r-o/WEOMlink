#include "displaylockguard.hpp"

#include "bsp/esp-bsp.h"

DisplayLockGuard::DisplayLockGuard()
{
    bool lock = bsp_display_lock(0);
    assert(lock);
}

DisplayLockGuard::~DisplayLockGuard()
{
    bsp_display_unlock();
}