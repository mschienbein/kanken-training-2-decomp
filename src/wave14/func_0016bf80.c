#include "game/display_animation.h"
#include "game/screen.h"


void func_0016bf80(struct DisplayObject *object, int index, float speed)
{
    struct DisplayAttachmentCommon *slot = object->attachments[index];
    slot->rate = speed;
}
