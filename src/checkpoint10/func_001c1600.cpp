#include "game/screen.h"

struct PairedDisplayHandle {
    DisplayObject *objects[2];
};

struct PairedDisplayOwner {
    unsigned reserved00;
    PairedDisplayHandle *handles[1];
};

/* INTEGRATOR: move to include/game/selection_cleanup.h */
extern "C" {
void func_00183208(DisplayObject *object);
void func_0017a938(const NameTable *const *tables, int count, int force);
extern const NameTable *data_0023f004[];
}

static inline void release_pair(PairedDisplayHandle *handle)
{
    if (handle) {
        for (int i = 0; i < 2; ++i) {
            if (handle->objects[i])
                func_00183208(handle->objects[i]);
        }
    }
}

static inline void reset_pair(PairedDisplayHandle *handle)
{
    if (handle) {
        handle->objects[0] = 0;
        handle->objects[1] = 0;
    }
}

extern "C" void func_001c1600(PairedDisplayOwner *owner)
{
    for (int i = 0; i < 1; ++i) {
        if (owner->handles[i]) {
            release_pair(owner->handles[i]);
            reset_pair(owner->handles[i]);
        }
    }
    func_0017a938(data_0023f004, -1, 0);
}
