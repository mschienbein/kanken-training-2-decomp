/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr snd_FxDelay.cpp,
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified: retain the retail 0.5f damping default in the canonical Param.
 */
#include <preinclude.h>
#include "nw/snd/snd_FxDelay.h"

namespace nw { namespace snd {
inline void FxDelay::FreeBuffer()
{
    for (int channel = 0; channel < m_ProcessChannelCount; ++channel)
        m_WorkBuffer.m_Delay[channel] = NULL;
}

FxDelay::FxDelay()
    : m_pBuffer(NULL), m_BufferSize(0), m_CurFrame(0), m_FeedbackGain(0),
      m_LpfCoef1(0x10000), m_LpfCoef2(0), m_ProcessChannelCount(4),
      m_IsActive(false)
{
    FreeBuffer();
    for (int channel = 0; channel < m_ProcessChannelCount; ++channel)
        m_WorkBuffer.m_Lpf[channel] = 0;
}
} }
