/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/snd/snd_FxReverb.cpp. Retail resets every configured
 * channel, rather than the older public implementation's fixed stereo pair.
 */
#include <preinclude.h>
/* The frozen standalone FxReverb header requires the SDK frame-size constant. */
enum { NN_SND_SAMPLES_PER_FRAME = 160 };
#include <nw/snd/snd_FxReverb.h>

namespace nw { namespace snd {
inline void FxReverb::FreeBuffer()
{
    for (int channel = 0; channel < m_ProcessChannelCount; ++channel) {
        m_WorkBuffer.m_EarlyReflection[channel] = 0;
        m_WorkBuffer.m_PreDelay[channel] = 0;
        m_WorkBuffer.m_CombFilter[channel][0] = 0;
        m_WorkBuffer.m_CombFilter[channel][1] = 0;
        m_WorkBuffer.m_AllPassFilter[channel] = 0;
    }
}
void FxReverb::Finalize()
{
    if (!m_IsActive)
        return;
    m_IsActive = false;
    FreeBuffer();
}
} }
