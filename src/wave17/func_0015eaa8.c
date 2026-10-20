#include "game/screen.h"

struct AnimationResetSlot {
    unsigned char reserved00[0x14];
    int state;
    float progress;
};

struct AnimationDisplayObject {
    struct DisplayObject base;
    struct AnimationResetSlot *slots[];
};

void func_0015eaa8(struct AnimationDisplayObject *object, int index)
{
    struct AnimationResetSlot *slot = object->slots[index];
    slot->state = 0;
    slot->progress = 0.0f;
}
