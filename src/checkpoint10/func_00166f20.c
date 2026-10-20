#include "game/screen.h"

#include "game/w11_control.h"
static void set_active(int enabled, struct DisplayObject *object)
{
    if (object == 0)
        return;
    switch (func_0017a9a0(object, 0)) {
    case 3: func_00195a08(object, 0, enabled); break;
    case 6: func_001826fc(object, enabled); break;
    case 5: func_0015e948(object, enabled); break;
    }
}

void func_00166f20(struct W11jVisualOwner *controller, int index, int enabled)
{
    struct W11jVisualNode *group = controller->nodes[index];
    int count;
    int i;
    if (group == 0)
        return;
    count = 10;
    if ((controller->formats[index].kind == 0 || controller->formats[index].kind == 6) && enabled)
        count = group->count;
    for (i = 0; i < count; ++i) {
        set_active(enabled, group->objects[i]);
    }
}
