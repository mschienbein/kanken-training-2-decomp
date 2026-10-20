#include "game/functions.h"
#include "game/screen.h"

void func_0015ea60(struct DisplayObject *object, int index, int operation)
{
    struct DisplayAttachmentCommon *slot = object->attachments[index];
    switch (operation) {
    case 0:
        slot->flags &= ~1u;
        break;
    case 1:
        slot->flags |= 1u;
        break;
    case 2:
        slot->flags ^= 1u;
        break;
    }
}
