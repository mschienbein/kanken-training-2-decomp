/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/snd/snd_FxDelay.cpp. Retail uses a running delay index,
 * a sample-relative input index and stores the channel low-pass history.
 */
#include <preinclude.h>
#include <nw/snd/snd_FxDelay.h>

namespace nw { namespace snd {
void FxDelay::UpdateBuffer(int numChannels, nn::snd::CTR::AuxBusData* data,
                          s32 sampleLength, SampleFormat format,
                          f32 sampleRate, OutputMode mode)
{
    if (!m_IsActive)
        return;
    s32* input[4];
    input[0] = data->frontLeft;
    input[1] = data->frontRight;
    input[2] = data->rearLeft;
    input[3] = data->rearRight;
    const u32 start = 160 * m_CurFrame;
    for (u32 channel = 0; channel < m_ProcessChannelCount; ++channel) {
        s32* samples = input[channel];
        s32* delay = m_WorkBuffer.m_Delay[channel];
        s32& history = m_WorkBuffer.m_Lpf[channel];
        u32 position = start;
        for (u32 sample = 0; sample < sampleLength; ++sample, ++position) {
            s32 previous = delay[position];
            s32 feedback = previous;
            if (feedback < 0) {
                s32 magnitude = -feedback;
                magnitude = (magnitude * m_FeedbackGain) >> 7;
                feedback = -magnitude;
            } else {
                feedback = (feedback * m_FeedbackGain) >> 7;
            }
            feedback = samples[sample] - feedback;
            s32 filtered = m_LpfCoef1 * feedback + m_LpfCoef2 * history;
            filtered >>= 7;
            history = filtered;
            delay[position] = filtered;
            samples[sample] = previous;
        }
    }
    if (++m_CurFrame >= m_DelayFrames)
        m_CurFrame = 0;
}
} }
