#include "game/resource_runtime.h"
#include <stddef.h>
#include "game/runtime.h"
#include "game/screen.h"



static void reset_text_slot(struct W24TextSlot *slot)
{
    if (slot != 0) {
        func_0019df08(slot->text, sizeof(slot->text));
        slot->color[3] = 255;
        slot->color[2] = 255;
        slot->position.y = 0.0f;
        slot->position.x = 0.0f;
        slot->scale.y = 1.0f;
        slot->scale.x = 1.0f;
        slot->color[1] = 255;
        slot->color[0] = 255;
        slot->object = 0;
    }
}

void func_001eaed8(struct W24TextScreen *screen)
{
    int i;
    for (i = 0; i < 32; ++i) {
        if (screen->slots[i] != 0) {
            if (screen->slots[i]->object != 0) func_00183208(screen->slots[i]->object);
            reset_text_slot(screen->slots[i]);
        }
    }
}
