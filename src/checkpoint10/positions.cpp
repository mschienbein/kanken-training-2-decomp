#include "game/character_panel.h"
#include "game/selection.h"

struct W10LayoutPosition {
    float x;
    float y;
};

/* INTEGRATOR: move to include/game/character_panel.h */
extern "C" {
extern const struct W10LayoutPosition *bss_00246f7c[];
extern void func_0016e300(struct WidgetController *controller, int index,
                        unsigned char red, unsigned char green,
                        unsigned char blue, unsigned char alpha);
void func_001b5a34(struct WidgetController *controller, int active);
}

static struct DisplayObject *slot_object(struct WidgetSlot *slot)
{
    return slot ? slot->object : 0;
}

static void set_object_position(struct DisplayObject *object, float x, float y)
{
    object->position[0] = x;
    object->position[1] = y;
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
}

static void set_slot_position(struct WidgetSlot *slot, float x, float y)
{
    if (slot) {
        /* ORIGINAL BUG: nonnull slots do not imply nonnull objects. */
        set_object_position(slot->object, x, y);
    }
}

static void show_object(struct DisplayObject *object, int enabled);

static void set_slot_flag(struct WidgetSlot *slot, unsigned mask)
{
    if (slot_object(slot))
        slot->state.flags |= mask;
}

void func_001b5a34(struct WidgetController *controller, int active)
{
    for (int index = 5; index <= 6; ++index) {
        set_slot_position(widget_slots(controller)[index], 0.0f, 3.0f);
        func_0016e7e0(controller, index, 1);
        set_slot_flag(widget_slots(controller)[index], 2);
    }
    if (active)
        func_0016e300(controller, 6, 255, 255, 255, 255);
    else
        func_0016e300(controller, 6, 255, 255, 255, 0);
}

void func_001b5cc0(struct CharacterPanel *controller, int index, int row)
{
    struct WidgetSlot *slot = controller->slots[index];
    struct DisplayObject *object = slot_object(slot);
    if (!object)
        return;
    const struct W10LayoutPosition *position = bss_00246f7c[index];
    float x = position->x;
    if (!func_00179148())
        x = -x;
    float y = position->y - static_cast<float>(row) * 33.0f;
    set_object_position(object, x, y);
    show_object(slot->object, 1);
}

static void show_object(struct DisplayObject *object, int enabled)
{
    if (!object)
        return;
    switch (func_0017a9a0(object, 0)) {
    case 3:
        func_00195a08(object, 0, enabled);
        break;
    case 5:
        func_0015e948(object, enabled);
        break;
    case 6:
        func_001826fc(object, enabled);
        break;
    }
}
