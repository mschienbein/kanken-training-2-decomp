#include "game/functions.h"
#include "game/runtime.h"

#include "game/render_resource.h"

inline void RenderResourceGroup::clearIds()
{
    for (unsigned i = 0; i < count; ++i)
        ids[i] = 0;
}

inline void RenderResourceGroup::finalize()
{
    if (initialized) {
        func_0019cc68(0);
        if (func_0019ceb4())
            func_0019f568();
        func_0019cb3c(count, ids);
        if (func_0019ceb4())
            func_0019f568();
        clearIds();
        initialized = 0;
    }
}

RenderResourceGroup *func_0016cb6c(RenderResourceGroup *group)
{
    group->vtable = &data_00224e04;
    group->finalize();
    return group;
}
