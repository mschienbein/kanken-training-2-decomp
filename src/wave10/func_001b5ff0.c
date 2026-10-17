#include "game/input.h"

int func_001b5ff0(struct InputSnapshot *input)
{
    int result = 0;
    if (input->state & 4)
        return result;
    if (!(input->state & 1))
        return result;
    if (input->state & 2) {
        result = 2;
        if (input->released || !func_001703c4(&input->rectangle, input->position.x, input->position.y)) {
            result = 1;
            input->state = 1;
        }
    } else if (input->active == 1 && input->previous_active == 0) {
        if (!func_001703c4(&input->rectangle, input->position.x, input->position.y))
            input->state = 1;
        else {
            result = 3;
            input->state |= 2;
        }
    }
    return result;
}
