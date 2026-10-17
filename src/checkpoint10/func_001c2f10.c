#include "game/pane_descriptors.h"

/* [INFERENCE] The elapsed timer and display share an eleven-slot controller. */
struct TimedDisplaySlot {
    float elapsed;
    struct DisplayObject *object;
};
struct TimedDisplayController {
    int selected;
    struct TimedDisplaySlot *slots[11];
};
extern void func_0016f534(struct DisplayObject *object, int index, float frame);

static void set_visibility(struct DisplayObject *object, int active)
{
    switch (func_0017a9a0(object, 0)) {
    case 3: func_00195a08(object, 0, active); break;
    case 6: func_001826fc(object, active); break;
    case 5: func_0015e948(object, active); break;
    }
}

static void reset_timer(struct TimedDisplaySlot *slot,
                        const struct NameTable *table, int active)
{
    if (active && table->count > 0)
        slot->elapsed = 2000.0f;
    else
        slot->elapsed = 0.0f;
}

void func_001c2f10(struct TimedDisplayController *controller, int index, int active)
{
    struct TimedDisplaySlot *slot = controller->slots[index];
    if (slot) {
        const struct NameTable *table = bss_00290dd8[index];
        struct DisplayObject *object;
        reset_timer(slot, table, active);
        object = slot->object;
        if (object) {
            set_visibility(object, active);
            if (table->count > 0) {
                func_0017a968(object, table, 0, 0);
                func_0016f534(object, 0, 0.0f);
            }
        }
    }
}
