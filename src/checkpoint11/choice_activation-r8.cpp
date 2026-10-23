#include <string.h>
#include "game/choice_pane_controllers.h"

static void activate_display(DisplayObject *object)
{
    unsigned type = func_0017a9a0(object, 0);
    if (type == 3) {
        func_00195a08(object, 0, 1);
        return;
    }
    if (type == 5)
        goto activate_type_five;
    if (type == 6)
        func_001826fc(object, 1);
    return;
activate_type_five:
    func_0015e948(object, 1);
}

static void initialize_choice_values(int *values)
{
    static const int initial_values[5] = { 1, -1, 0, 0, 0 };
    memcpy(values, initial_values, sizeof(initial_values));
}

void func_001b376c(ChoiceGridState *controller, int index)
{
    ChoiceEntry *entry = &controller->entries[index];
    if (entry->objects[0]) {
        int i = 0;
        int values[5];
        initialize_choice_values(values);
        for (; i < 5; ++i) {
            DisplayObject *object = entry->objects[i];
            if (!object) continue;
            if (i == 2) {
                if (entry->flags & 0x10) continue;
            } else if (i == 3) {
                if (entry->flags & 0x20) continue;
            } else if (i == 4) {
                if (entry->flags & 0x40) continue;
            }
            if (values[i] != -1)
                func_0017a968(object, entry->tables[i], values[i], 1);
            activate_display(object);
        }
        entry->remaining = 400.0f;
        entry->selected = -1;
        entry->state = 2;
        entry->result = -1;
    }
}
