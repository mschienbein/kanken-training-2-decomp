#include "game/screen.h"


void func_00195a08(struct DisplayObject *object, int index, int active)
{
    struct DisplayAttachmentCommon *slot = object->attachments[index];
    if (active == 0) {
        slot->flags &= ~2u;
    } else if (active == 1) {
        slot->flags |= 2u;
    } else if (active == 2) {
        slot->flags ^= 2u;
    }
}
