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
    void finalize();
    void clearIds();
};

/* INTEGRATOR: move to include/game/render_resource.h */
extern "C" {
extern const RenderGroupVTable data_00224e04;
int func_0019ceb4(void);
void func_0019cc68(unsigned id);
void func_0019cb3c(int count, const unsigned *ids);
RenderResourceGroup *func_0016cb6c(RenderResourceGroup *group);
}

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
