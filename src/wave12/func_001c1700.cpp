#include "game/animation.h"
#include "game/prompt.h"
#include "game/property_descriptor.h"
#include "game/screen.h"



static inline void start_prompt(struct PromptManager *manager)
{
    if (manager->objects != 0) {
        struct DisplayObject *object = manager->objects[0];
        const struct NameTable *table = data_0023f004[0];
        if (object != 0 && table->count > 0)
            func_0017a968(object, table, 0, 0);
    }
}

extern "C" void func_001c1700(struct PromptManager *manager)
{
    if (manager->delay > 0.0f) {
        manager->delay -= func_00191134() * 0.001f;
        if (manager->delay <= 0.0f) {
            manager->delay = 0.0f;
            start_prompt(manager);
        }
    }
}
