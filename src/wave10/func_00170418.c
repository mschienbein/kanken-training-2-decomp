#include <stdbool.h>
#include <string.h>

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

struct TouchDeviceState {
    unsigned char reserved00[0x0c];
    unsigned int active;
    unsigned char reserved10[0x14];
    unsigned int released;
    unsigned char reserved28[0x10];
    struct InputPoint position;
};

extern struct TouchDeviceState bss_004972d8;

void func_00170418(struct InputSnapshot *input)
{
    struct InputPoint position;
    bool active, released, tracking;
    input->previous_active = input->active;
    input->previous_released = input->released;
    /* Retain the previous sample's coordinate representations without conversion. */
    memcpy(&input->previous_position.x, &input->position.x, sizeof(float));
    memcpy(&input->previous_position.y, &input->position.y, sizeof(float));
    active = bss_004972d8.active != 0;
    input->active = active;
    released = bss_004972d8.released != 0;
    input->released = released;
    position = bss_004972d8.position;
    position.x += -160.0f;
    position.y += -120.0f;
    position.y = -position.y;
    input->position = position;
    tracking = input->tracking;
    if (tracking) {
        if (!released)
            return;
        tracking = false;
set_tracking:
        input->tracking = tracking;
        return;
    }
    if (active) {
        input->start_position = position;
        tracking = true;
        goto set_tracking;
    }
}
