#include "game/audio.h"
#include "game/display_animation.h"
#include "game/screen.h"



int func_00195874(const struct DisplayObject *object, int index)
{
    const struct DisplayAnimationTrack *track =
        ((const struct AnimatedObject *)object)->tracks[index];
    switch (track->mode) {
    case 1:
        if (track->rate < 0.0f) {
            if (track->position >= track->end)
                return 0;
        } else if (track->rate > 0.0f) {
            if (track->position <= track->end)
                return 0;
        }
        break;
    case 2:
        if (track->flags & 0x10)
            return 0;
        break;
    }
    return 1;
}
