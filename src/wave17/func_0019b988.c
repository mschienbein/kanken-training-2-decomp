#include "game/functions.h"
#include "game/runtime.h"

#include "game/render_resource.h"



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
