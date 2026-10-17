#include <stdbool.h>

struct InputRectangle {
    float x, y, width, height;
};

struct InputPoint {
    float x, y;
};

struct InputSnapshot {
    bool active, released;
    unsigned char reserved02[2];
    struct InputPoint position;
    bool previous_active, previous_released;
    unsigned char reserved0e[2];
    struct InputPoint previous_position;
    bool tracking;
    unsigned char reserved19[3];
    struct InputPoint start_position;
    struct InputRectangle rectangle;
    unsigned int state;
};

extern int func_001703c4(const struct InputRectangle *rectangle, float x, float y);

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
