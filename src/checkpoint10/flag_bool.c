#include "game/screen.h"

struct W6DisplayHandle {
    struct DisplayObject *object;
    int initialized;
};

static void set_active(int active, struct DisplayObject *object)
{
    if (object == 0)
        return;
    switch (func_0017a9a0(object, 0)) {
    case 3: func_00195a08(object, 0, active); break;
    case 6: func_001826fc(object, active); break;
    case 5: func_0015e948(object, active); break;
    }
}

static __forceinline void update_handle(struct W6DisplayHandle *handle, int active, int initialized)
{
    set_active(active, handle->object);
    handle->initialized = initialized;
}

void func_00173dac(struct W6DisplayHandle **handles, int index, int active)
{
    struct W6DisplayHandle *handle = handles[index];
    if (handle) {
        int initialized = handle->initialized != 0;
        if (initialized) update_handle(handle, active, initialized);
    }
}
