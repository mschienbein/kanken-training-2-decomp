#include "game/screen.h"

struct W6DisplayCollection {
    unsigned char reserved00[0x18];
    struct DisplayObject **objects;
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

void func_001c2b9c(struct W6DisplayCollection *collection, int active)
{
    int enabled = active != 0;
    struct DisplayObject **objects = collection->objects;
    int i;
    if (objects == 0)
        return;
    for (i = 0; i < 63; ++i)
        set_active(enabled, objects[i]);
}
