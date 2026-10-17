#include "game/screen.h"

struct W8DisplayHandle {
    struct DisplayObject *object;
    unsigned char reserved04[8];
    float scale_x;
    float scale_y;
};

struct W8DisplaySet {
    unsigned char reserved00[0x1300];
    struct W8DisplayHandle *handles[];
};

/* INTEGRATOR: move to include/game/display_transform.h */
extern void func_001826fc(struct DisplayObject *object, int active);
extern void func_00195a08(struct DisplayObject *object, int index, int active);
extern void func_0015e948(struct DisplayObject *object, int active);

static __forceinline void set_handle_active(struct W8DisplayHandle *handle, int active)
{
    struct DisplayObject *object = handle->object;
    if (object == 0)
        return;
    switch (func_0017a9a0(object, 0)) {
    case 3:
        func_00195a08(object, 0, active);
        break;
    case 5:
        func_0015e948(object, active);
        break;
    case 6:
        func_001826fc(object, active);
        break;
    }
}

void func_0017a0e0(struct W8DisplaySet *set, int index, int active)
{
    if (set->handles[index] != 0)
        set_handle_active(set->handles[index], active);
}
