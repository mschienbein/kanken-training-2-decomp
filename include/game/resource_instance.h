#ifndef GAME_RESOURCE_INSTANCE_H
#define GAME_RESOURCE_INSTANCE_H

#include "game/heap_interface.h"

/* Both allocation and release consumers select this same pair of heap owners. */
struct AllocatorPair {
    struct Heap *primary;
    struct Heap *secondary;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct AllocatorPair data_00242f84;
void *func_00116cb4(unsigned size, unsigned alignment);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <preinclude.h>
#include <nw/font/font_RectDrawer.h>

/* [INFERENCE] Target-version layout drawer prefix: the original forwards this
 * unchanged receiver to both canonical RectDrawer initialization overloads.
 * Only the observed extension after the pinned 0x688-byte base is exposed;
 * this does not claim the public nw::lyt::Drawer has this target layout. */
struct TargetLayoutDrawer : nw::font::RectDrawer {
    unsigned char reserved688[0x0c];
    unsigned state694;
    unsigned state698;
    unsigned state69c;
    unsigned char enabled;
    unsigned char ready;
    unsigned char mode;
};
struct ShaderBinaryInput {
    const void *binary;
    unsigned size;
};
extern "C" void func_0010e230(TargetLayoutDrawer *drawer,
                              const ShaderBinaryInput *shader, void *vertex_buffer);
#endif

#endif
