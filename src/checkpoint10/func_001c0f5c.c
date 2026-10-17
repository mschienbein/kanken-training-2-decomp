#include "game/command_state.h"
#include "game/resource_runtime.h"
#include <stddef.h>
#include <string.h>
#include "game/property_descriptor.h"




void func_001c0f5c(struct WidgetSlot **slots)
{
    int i;
    for (i = 0; i < 3; ++i) {
        if (slots[i] != 0) {
            if (slots[i]->object != 0)
                func_00183208(slots[i]->object);
            {
                struct WidgetSlot *current = slots[i];
                if (current != 0) {
                    memset(current, 0, offsetof(struct WidgetSlot, text));
                }
            }
        }
    }
    func_0017a938(bss_00290e60, -1, 0);
}
