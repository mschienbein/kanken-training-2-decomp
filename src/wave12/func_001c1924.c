#include "game/audio.h"
#include "game/prompt.h"
#include "game/property_descriptor.h"
#include "game/screen.h"



int func_001c1924(const struct PromptManager *manager)
{
    if (manager->handles[0] != 0) {
        struct ResourceEvent *event = manager->handles[0]->objects[0];
        const struct NameTable *table = data_0023f004[0];
        if (event != 0 && table->count > 0) {
            if (func_00195874(&event->base, 0) != 0)
                return 0;
        }
    }
    return 1;
}
