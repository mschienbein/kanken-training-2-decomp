#include "game/screen.h"

void func_0015eaa8(struct DisplayObject *object, int index)
{
    struct DisplayTimedAttachment *slot = display_timed_attachment(object, index);
    slot->mode = 0;
    slot->duration = 0.0f;
}
