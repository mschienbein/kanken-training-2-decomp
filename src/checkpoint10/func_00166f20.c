#include "game/screen.h"

/* INTEGRATOR: share the numeric-display types in include/game/number_display.h. */
struct W15gNumberFormat {
    unsigned kind;
    unsigned reserved04[6];
};
struct W15gNumberGroup {
    unsigned reserved00;
    struct DisplayObject *objects[10];
    short count;
    unsigned char reserved2e[0x52];
    float position[2];
    unsigned char reserved88[8];
    float scale[2];
    unsigned char color[4];
};
struct W15gNumberController {
    unsigned char reserved00[0xc];
    struct W15gNumberFormat formats[72];
    struct W15gNumberGroup *groups[];
};

/* INTEGRATOR: differs from src/wave4/func_00166f1c.c: the group-array access proves a typed numeric controller, not void *. */
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

void func_00166f20(struct W15gNumberController *controller, int index, int enabled)
{
    struct W15gNumberGroup *group = controller->groups[index];
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
