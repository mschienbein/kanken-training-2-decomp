#include "game/screen.h"

void func_0016f534(struct DisplayObject *object, int index, float duration)
{
    struct DisplayTimedAttachment *binding = display_timed_attachment(object, index);
    if (duration == -1.0f) {
        binding->mode = 2;
    } else {
        binding->duration = duration;
        binding->mode = 1;
    }
}
