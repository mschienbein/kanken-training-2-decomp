#include "game/audio_voice.h"
void nw::snd::internal::CurveAdshr::SetHold(int value)
{
    int square = (value + 1) * (value + 1);
    m_Hold = square / 4;
}
