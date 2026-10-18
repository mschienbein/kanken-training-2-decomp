/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/snd/snd_FxDelay.cpp. Finalization inlines the native buffer
 * reset; the complete parameter record is cleared before the delay pointers.
 */
#include <preinclude.h>
#include <cstring>
#include <nw/snd/snd_FxDelay.h>

namespace nw { namespace snd {
inline void FxDelay::FreeBuffer()
{
    for (int channel = 0; channel < m_ProcessChannelCount; ++channel)
        m_WorkBuffer.m_Delay[channel] = 0;
}
void FxDelay::Finalize()
{
    if (!m_IsActive)
        return;
    m_IsActive = false;
    std::memset(&m_Param, 0, sizeof(m_Param));
    FreeBuffer();
}
} }
