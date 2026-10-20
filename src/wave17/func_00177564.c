#include "game/widget_animation.h"

int func_00177564(const struct AnimationBank *bank, int index)
{
    const struct WidgetAnimationTrack *track = &bank->tracks[index];
    if (track->present != 0)
        return track->state == 4;
    return 0;
}
