/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned declaration-only interface, derived from public NW4C uses at
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * No Nintendo SDK header consulted or copied. Opaque synchronization object
 * layouts are deliberately not guessed here; dependants needing them stay blocked.
 */
#pragma once
#include "types.h"
namespace nn { namespace os {
uptr GetDeviceMemoryAddress();
size_t GetDeviceMemorySize();
class CriticalSection;
class InterCoreCriticalSection;
class LightEvent;
class BlockingQueue;
class InterCoreBlockingQueue;
class Thread;
/* [INFERENCE] Tick wraps the public signed 64-bit svcGetSystemTick quantity:
 * https://www.3dbrew.org/wiki/SVC#GetSystemTick . Only declaration-level use
 * is provided; no unseen SDK arithmetic/conversion implementation is invented. */
class Tick {
    s64 m_Value;
public:
    Tick();
    explicit Tick(s64);
    static Tick GetSystemCurrent();
    s64 ToMicroSeconds() const;
    Tick& operator+=(const Tick&);
    Tick& operator-=(const Tick&);
};
Tick operator+(const Tick&, const Tick&);
Tick operator-(const Tick&, const Tick&);
namespace ARM { void DataMemoryBarrier(); }
} }
