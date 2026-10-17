#include "game/runtime.h"



void func_0016ec28(struct MotionState *animation, int frame)
{
    animation->mode.value = 2;
    if (frame != -1) {
        animation->current = frame;
        animation->time = animation->allocation[frame].time;
        animation->first = frame;
        animation->first_time = animation->allocation[frame].time;
        func_0016e924(animation, 0.0f);
    }
}
