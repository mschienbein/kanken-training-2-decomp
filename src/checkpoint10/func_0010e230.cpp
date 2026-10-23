#include "game/resource_instance.h"

extern "C" void func_0010e230(TargetLayoutDrawer *instance,
                             const ShaderBinaryInput *descriptor, void *shared_table)
{
    unsigned size = nw::font::RectDrawer::GetCommandBufferSize(descriptor->binary, descriptor->size);
    unsigned extra = shared_table == 0 ? 0x9c0 : 0;
    void *workspace = func_00116cb4(size + extra, 4);
    if (shared_table == 0) {
        const void *file = descriptor->binary;
        unsigned file_size = descriptor->size;
        instance->nw::font::RectDrawer::Initialize(workspace, file, file_size);
    } else {
        const void *file = descriptor->binary;
        unsigned file_size = descriptor->size;
        instance->nw::font::RectDrawer::Initialize(shared_table, workspace, file, file_size);
    }
    instance->state694 = 0;
    instance->state698 = 0;
    instance->state69c = 0;
    instance->enabled = 1;
    instance->ready = 1;
    instance->mode = 6;
}
