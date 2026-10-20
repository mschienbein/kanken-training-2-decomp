#include "game/functions.h"
#include "game/runtime.h"

struct RenderGroupVTable;
struct RenderResourceGroup {
    const struct RenderGroupVTable *vtable;
    unsigned char initialized;
    unsigned char reserved05[3];
    unsigned mode;
    unsigned count;
    unsigned ids[3];
    int width;
    int height;
};
struct RenderResourceConfiguration {
    unsigned reserved00;
    unsigned mode;
    unsigned count;
    unsigned format;
    int width;
    int height;
    unsigned memory_type;
};

extern void func_00196114(unsigned mode);
extern void func_0011dbb4(int count, unsigned *ids);
extern void func_0019cc68(unsigned id);
extern void func_0011dab4(unsigned format, int width, int height, unsigned memory_type);



void func_0019b988(const struct RenderResourceConfiguration *configuration,
                   struct RenderResourceGroup *group)
{
    unsigned i;
    if (group->initialized)
        return;
    if (configuration->count != 2)
        func_0019f568();
    group->mode = configuration->mode;
    group->count = configuration->count;
    group->width = configuration->width;
    group->height = configuration->height;
    func_00196114(configuration->mode);
    func_0011dbb4(group->count, group->ids);
    for (i = 0; i < group->count; ++i) {
        func_0019cc68(group->ids[i]);
        func_0011dab4(configuration->format, configuration->width,
                       configuration->height, configuration->memory_type);
        if (func_0019ceb4())
            func_0019f568();
    }
    func_0011da90(0, 0);
    group->initialized = 1;
    if (func_0019ceb4())
        func_0019f568();
}
