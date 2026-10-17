#include "game/runtime.h"


void func_0016ec6c(struct MotionState *animation, int looping)
{
    unsigned flags = animation->flags;
    animation->flags = looping ? flags & ~0x10u : flags | 0x10u;
}
