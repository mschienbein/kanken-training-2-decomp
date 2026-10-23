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
        if (input->pointer.released || !func_001703c4(&input->rectangle, input->pointer.position.x, input->pointer.position.y)) {
            result = 1;
            input->state = 1;
        }
    } else if (input->pointer.active == 1 && input->pointer.previous_active == 0) {
        if (!func_001703c4(&input->rectangle, input->pointer.position.x, input->pointer.position.y))
            input->state = 1;
        else {
            result = 3;
            input->state |= 2;
        }
    }
    return result;
}
