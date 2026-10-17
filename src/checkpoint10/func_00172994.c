#include "game/task_runtime.h"
struct PlaybackSlot {
    int resource;
    void *object;
};

struct PlaybackRegistry {
    int ready;
    unsigned char reserved04[0x2f67c];
    struct PlaybackSlot entries[];
};



int func_00172994(int index)
{
    if (!bss_0029f540.ready)
        return 0;
    return bss_0029f540.entries[index].resource;
}
