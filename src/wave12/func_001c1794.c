#include "game/audio.h"
#include "game/prompt.h"
#include "game/property_descriptor.h"
#include "game/screen.h"



void func_001c1794(struct PromptManager *manager)
{
    if (manager->handles[0] != 0) {
        struct ResourceEvent *event = manager->handles[0]->objects[1];
        const struct NameTable *table = data_0023f004[0];
        if (event != 0 && table->count > 0)
            func_0017a968(&event->base, table, 0, 0);
    }
    manager->delay = 8.0f;
}
