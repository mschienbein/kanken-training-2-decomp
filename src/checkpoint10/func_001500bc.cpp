#define NW_RELEASE
#include "game/audio_voice.h"
#include "nw/ut/ut_Inlines.h"

/* INTEGRATOR: move this recovered declaration into game/audio_voice.h. */
extern "C" void func_001500bc(AudioVoiceGroup *group, int type, float factor);

void func_001500bc(AudioVoiceGroup *group, int type, float factor)
{
    const float maximum = 1.0f;
    const float minimum = 0.0f;
    factor = nw::ut::Clamp(factor, minimum, maximum);
    bool changed = false;
    if (group->reserved22 != type) {
        group->reserved22 = type;
        changed = true;
    }
    if (group->value3c != factor) {
        group->value3c = factor;
        changed = true;
    }
    if (changed)
        group->dirty |= 0x20;
}
