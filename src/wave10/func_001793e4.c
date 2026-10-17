#include "game/screen.h"

struct QueuedDisplayHandle {
    struct BoundDisplayObject *object;
    unsigned int flags;
};

struct QueuedDisplayState {
    unsigned char reserved00[0x28];
    struct QueuedDisplayHandle *handle;
};


void func_001793e4(struct QueuedDisplayState *state, int active)
{
    struct QueuedDisplayHandle *handle = state->handle;
    struct BoundDisplayObject *object;
    if (!handle || !handle->object)
        return;
    func_001963f8(handle->object);
    if (active) {
        if (state->handle && state->handle->object)
            state->handle->flags &= ~1u;
        object = handle->object;
        object->base.position[2] = 2.7f;
        switch (func_0017a9a0(&object->base, 0)) {
        case 6:
            func_00181700(object, object->base.position[0], object->base.position[1], object->base.position[2]);
            break;
        case 5:
            func_0015e684(object, 0, object->base.position[0], object->base.position[1], object->base.position[2]);
            break;
        }
    } else {
        if (state->handle && state->handle->object)
            state->handle->flags |= 1;
        object = handle->object;
        object->base.position[2] = 0.6f;
        switch (func_0017a9a0(&object->base, 0)) {
        case 6:
            func_00181700(object, object->base.position[0], object->base.position[1], object->base.position[2]);
            break;
        case 5:
            func_0015e684(object, 0, object->base.position[0], object->base.position[1], object->base.position[2]);
            break;
        }
    }
    func_00181448(handle->object);
}
