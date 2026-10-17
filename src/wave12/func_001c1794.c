#include "game/audio.h"
#include "game/prompt.h"
#include "game/property_descriptor.h"
#include "game/screen.h"



void func_001c1794(struct PromptManager *manager)
{
    if (manager->objects != 0) {
        struct DisplayObject *object = manager->objects[1];
        const struct NameTable *table = data_0023f004[0];
        if (object != 0 && table->count > 0)
            func_0017a968(object, table, 0, 0);
    }
    manager->delay = 8.0f;
}
