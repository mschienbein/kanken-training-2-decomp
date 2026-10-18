#include "game/screen.h"
#include "game/pane_descriptors.h"

struct W16ResourceHandle {
    struct DisplayObject *object;
    const struct NameTable *resource;
};

struct W16ResourceSlots {
    unsigned char reserved00[0xc];
    struct W16ResourceHandle *handles[];
};

extern struct DisplayObject *func_0017a818(const struct NameTable *table, int screen,
                                         int active, float x, float y, float z);

void func_001702fc(struct W16ResourceSlots *slots, int index, int resource)
{
    struct W16ResourceHandle *handle = slots->handles[index];
    if (handle != 0 && handle->object == 0) {
        const struct NameTable *table = bss_0024716c[resource];
        float z;
        int screen = 3;
        int active = 0;
        handle->resource = table;
        switch (index) {
        case 0: z = 0.5f; break;
        case 1: z = 0.7f; break;
        case 2: z = 1.0f; break;
        default: z = 1.3f; screen = 4; break;
        }
        handle->object = func_0017a818(table, screen, active, 0.0f, 0.0f, z);
    }
}
