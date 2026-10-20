#include "game/audio.h"
#include "game/display_animation.h"
#include "game/screen.h"



int func_00195874(const struct DisplayObject *object, int index)
{
    const struct DisplayTimedAttachment *track =
        display_timed_attachment_const(object, index);
    switch (track->mode) {
    case 1:
        if (track->common.rate < 0.0f) {
            if (track->duration >= track->common.value)
                return 0;
        } else if (track->common.rate > 0.0f) {
            if (track->duration <= track->common.value)
                return 0;
        }
        break;
    case 2:
        if (track->common.flags & 0x10)
            return 0;
        break;
    }
    return 1;
}
