#include "game/functions.h"
#include "game/screen.h"
#include "game/pane_descriptors.h"

extern void func_0015eac8(struct FlagDisplayObject *object, int index, float value);

extern void func_001701b4(struct Screen *screen, int index, int active);

void func_001af154(struct Screen *screen, int current, int total)
{
    struct DisplayObject *object = screen->slots[3]->object;
    if (object && total != 0) {
        int percentage = (int)((float)current / (float)total * 100.0f);
        func_0017a968(object, bss_0024716c[10], 0, 0);
        func_0015eac8((struct FlagDisplayObject *)object, 0, (float)percentage);
        func_0015ea60((struct FlagDisplayObject *)object, 0, 0);
        object->position[0] = 1.0f;
        object->position[1] = 21.0f;
        switch (func_0017a9a0(object, 0)) {
        case 6:
            func_00181700((struct BoundDisplayObject *)object,
                         object->position[0], object->position[1], object->position[2]);
            break;
        case 5:
            func_0015e684((struct BoundDisplayObject *)object, 0,
                         object->position[0], object->position[1], object->position[2]);
            break;
        }
        func_001701b4(screen, 3, 1);
    }
}
