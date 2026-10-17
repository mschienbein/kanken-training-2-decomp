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
struct W24eSoundLoadWorker {
    unsigned char reserved00[8];
    int tag_count;
    int tags[48];
    unsigned char reservedcc[0x2f5dc];
    struct W24eSoundLoadContext *sound;
    struct RegionHeapOwner *heap;
    unsigned char reserved2f6b0[0x205c];
    int pending;
    int exiting;
    unsigned char reserved31714[0x1014];
    unsigned requests[48];
    int write;
    int read;
    int release_marker;
};

extern int func_00116ce8(struct RegionHeapOwner *owner);
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
                                   bss_0029f540.requests[bss_0029f540.read],
                                   bss_0029f540.heap, -1, 0);
                }
                bss_0029f540.requests[bss_0029f540.read] = 0;
                ++bss_0029f540.read;
                if (bss_0029f540.read >= 48)
                    bss_0029f540.read = 0;
                if (bss_0029f540.release_marker == bss_0029f540.read)
                    bss_0029f540.release_marker = -1;
            } while (bss_0029f540.read != bss_0029f540.write);
            bss_0029f540.pending = 0;
        }
    }
}
