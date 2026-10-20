#include "game/audio_voice.h"
void nw::snd::internal::CurveAdshr::SetRelease(int value)
{
    float result;
    if (value == 127)
        result = 65535.0f;
    else if (value == 126)
        result = 24.0f;
    else if (value < 50)
        result = ((float)(2 * value + 1) * (1.0f / 128.0f)) * 0.2f;
    else
        result = (60.0f / (float)(126 - value)) * 0.2f;
    m_Release = result;
}
