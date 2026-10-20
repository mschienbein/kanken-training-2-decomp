#include "game/task_runtime.h"
#include "game/synchronization.h"
#include "game/region_heap.h"
#include "game/archive.h"

struct W24eSoundLoader {
    const void *vtable;
    struct SoundArchivePlayerView *player;
};
struct W24eSoundLoadContext {
    unsigned char reserved00[0x0c];
    struct W24eSoundLoader loader;
};

extern int func_00192ec8(struct W24eSoundLoader *loader, unsigned sound,
                         struct RegionHeapOwner *heap, int offset, int flags);

void func_0010a704(struct SignalState *signal)
{
    for (;;) {
        func_00195d94(signal);
        if (bss_0029f540.exiting)
            return;
        if (bss_0029f540.pending) {
            do {
                if (bss_0029f540.release_marker == -1) {
                    if (!bss_0029f540.tags[bss_0029f540.tag_count])
                        bss_0029f540.tags[bss_0029f540.tag_count] =
                            func_00116ce8(bss_0029f540.heap);
                    ++bss_0029f540.tag_count;
                    func_00192ec8(&bss_0029f540.sound->loader,
                                   bss_0029f540.values[bss_0029f540.read_index],
                                   bss_0029f540.heap, -1, 0);
                }
                bss_0029f540.values[bss_0029f540.read_index] = 0;
                ++bss_0029f540.read_index;
                if (bss_0029f540.read_index >= 48)
                    bss_0029f540.read_index = 0;
                if (bss_0029f540.release_marker == bss_0029f540.read_index)
                    bss_0029f540.release_marker = -1;
            } while (bss_0029f540.read_index != bss_0029f540.write_index);
            bss_0029f540.pending = 0;
        }
    }
}
