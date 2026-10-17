#include "game/resource_instance.h"
/* [INFERENCE] Role: initializes a game-side RectDrawer-derived instance from a shader
 * binary descriptor.  func_00116e30 is the command-buffer size query, func_00116d18 the
 * RectDrawer::Initialize(void *buffer, const void *shaderBinary, u32 size) overload that
 * owns vertex and command storage in one buffer, func_00116dc0
 * RectDrawer::Initialize(void *vertexBuffer, void *commandBuffer, const void *shaderBinary,
 * u32 size); func_00116cb4 allocates the work buffer (size, alignment).  0x9c0 is the
 * vertex-buffer footprint added when no shared vertex buffer is supplied. */
struct ResourceFile {
    unsigned signature;
    unsigned count_offset_words;
    unsigned metadata_offset;
};
struct ResourceInstance {
    unsigned char reserved00[0x694];
    unsigned state694;
    unsigned state698;
    unsigned state69c;
    unsigned char enabled;
    unsigned char ready;
    unsigned char mode;
};
struct ResourceDescriptor {
    const struct ResourceFile *file;
    unsigned file_size;
};

extern unsigned func_00116e30(const struct ResourceFile *file, unsigned file_size);

extern void func_00116d18(struct ResourceInstance *instance, void *workspace,
                          const struct ResourceFile *file, unsigned file_size);
extern void func_00116dc0(struct ResourceInstance *instance, void *shared_table,
                          void *workspace, const struct ResourceFile *file, unsigned file_size);

void func_0010e230(struct ResourceInstance *instance,
                   const struct ResourceDescriptor *descriptor, void *shared_table)
{
    unsigned size = func_00116e30(descriptor->file, descriptor->file_size);
    unsigned extra = shared_table == 0 ? 0x9c0 : 0;
    void *workspace = func_00116cb4(size + extra, 4);
    if (shared_table == 0) {
        const struct ResourceFile *file = descriptor->file;
        unsigned file_size = descriptor->file_size;
        func_00116d18(instance, workspace, file, file_size);
    } else {
        const struct ResourceFile *file = descriptor->file;
        unsigned file_size = descriptor->file_size;
        func_00116dc0(instance, shared_table, workspace, file, file_size);
    }
    instance->state694 = 0;
    instance->state698 = 0;
    instance->state69c = 0;
    instance->enabled = 1;
    instance->ready = 1;
    instance->mode = 6;
}
