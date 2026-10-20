#include "game/widget_animation.h"

void func_00177adc(struct AnimationBank *bank, int index, int enabled)
{
    struct WidgetAnimationTrack *track = &bank->tracks[index];
    if (track->present != 0) {
        if (enabled == 0)
            track->flags |= 1;
        else
            track->flags &= ~1u;
    }
}
