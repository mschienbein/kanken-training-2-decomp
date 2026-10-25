#include "game/render_state.h"
#include <string.h>


static void detach_resource(struct Renderbuffer *resource)
{
    struct Framebuffer *block;
    struct FramebufferManager *registry = data_002426d8;
    for (block = registry->framebuffers; block; block = block->previous) {
        int i;
        for (i = 0; i < 2; ++i) {
            struct FramebufferAttachment *slot = &block->attachments[i];
            if (slot->resource == resource) {
                if (registry->bound_framebuffer == block)
                    data_00242fd0->dirty |= 1;
                slot->resource = 0;
                memset(slot, 0, 3 * sizeof(unsigned));
            }
        }
    }
}

/* [INFERENCE] Cleanup is factored into detach/release operations;
 * the original compiler-inlined helper identities are unrecovered. */
static inline void release_resource(struct Renderbuffer *resource)
{
    if (resource) {
        detach_resource(resource);
        if (resource->memory && data_002429f8)
            data_002429f8(resource->module, 0x103, resource->handle, resource->memory);
        if (data_002429f8)
            data_002429f8(0x10000, 0x100, 0, resource);
    }
}

static inline void release_block(struct Framebuffer *block)
{
    if (block && data_002429f8)
        data_002429f8(0x10000, 0x100, 0, block);
}

void func_00117f2c(void)
{
    struct Renderbuffer *resource;
    struct Framebuffer *block;
    if (!data_002426d8)
        return;
    resource = data_002426d8->renderbuffers;
    while (resource) {
        struct Renderbuffer *next = resource->next;
        release_resource(resource);
        resource = next;
    }
    block = data_002426d8->framebuffers;
    while (block) {
        struct Framebuffer *next = block->previous;
        release_block(block);
        block = next;
    }
    data_002426d8 = 0;
}
