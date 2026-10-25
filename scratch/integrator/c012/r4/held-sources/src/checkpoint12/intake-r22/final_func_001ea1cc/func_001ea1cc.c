#include "game/w11_control.h"
#include <string.h>

/* Shared descriptor table comes from game/screen.h. */
/* Position and scale forwarding use the shared display declarations. */

static void forward_position(struct DisplayObject *object)
{
    switch (func_0017a9a0(object, 0)) {
    case 6:
        func_00181700((struct BoundDisplayObject *)object, object->position[0], object->position[1], object->position[2]);
        break;
    case 5:
        func_0015e684((struct BoundDisplayObject *)object, 0, object->position[0], object->position[1], object->position[2]);
        break;
    }
}

static void forward_scale(struct DisplayObject *object)
{
    switch (func_0017a9a0(object, 0)) {
    case 6:
        func_001816e0((struct BoundDisplayObject *)object, object->scale[0], object->scale[1]);
        break;
    case 5:
        func_0015e474(object, 0, object->scale[0], object->scale[1], object->scale[2]);
        break;
    }
}

static void update_number(int value, const struct NameTable *table,
                          struct W11jVisualNode *group, int offset)
{
    struct DisplayObject *object = group->objects[0];
    if (object) {
        unsigned y;
        func_0017a968(object, table, value, 1);
        memcpy(&y, &group->position[1], sizeof(y));
        object->position[0] = group->position[0] + offset;
        memcpy(&object->position[1], &y, sizeof(y));

        forward_position(object);

        memcpy(object->scale, &group->scale_x, 2 * sizeof(float));
        object->scale[2] = 1.0f;
        forward_scale(object);
        func_00170088(object, group->color[0], group->color[1], group->color[2], group->color[3]);
    }
}

void func_001ea1cc(struct W11jVisualOwner *controller, int index, const struct W11jNumberFormat *format, int value)
{
    struct W11jVisualNode *group = controller->nodes[index];
    int offset = 0;
    if (format->flags & 0x10) {
        if (value == 1 || value == 3)
            offset = 23;
        else if (value == 11)
            offset = 10;
    }
    update_number(value + 1, bss_00245344[format->kind], group, offset);
}
