#include "game/property_descriptor.h"
#include "game/category_progress.h"

void func_0016bf90(struct L044TrainingFrame *frame, int index, int resource)
{
    struct WidgetSlot *handle = frame->slots[index];
    if (handle == 0) return;
    if (handle->object != 0) return;
    {
        float z = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        int screen = 4;
        int active = 0;
        switch (resource) {
        case 0: case 1: z = 0.5f; break;
        case 2: z = 0.6f; break;
        case 3: case 4: z = 2.0f; break;
        default: break;
        }
        handle->object = func_0017a818(bss_00290f60[resource], screen, active, x, y, z);
    }
}
