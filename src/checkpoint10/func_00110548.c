#include "game/bss_0049ecf0.h"
/* INTEGRATOR: differs from include/game/bss_0049ecf0.h: queue words 0x28 and
 * 0x2c are the execution threshold and queue mode. Other definitions agree. */
struct Command {
    unsigned char kind, reserved01, ready, reserved03;
    const void *source;
    unsigned size;
    unsigned flags;
    unsigned char reserved10[12];
};
struct CommandQueue {
    unsigned char reserved00[4];
    unsigned char *base;
    unsigned char reserved08[4];
    unsigned offset;
    unsigned char reserved10[4];
    unsigned committed_offset;
    struct Command *commands;
    unsigned char reserved1c[4];
    int count;
    int completed_count;
    int execution_threshold;
    unsigned mode;
};
struct CommandSystem {
    unsigned char reserved00[0x11];
    unsigned char pending, enabled;
    unsigned char reserved13[5];
    unsigned char ready;
    unsigned char reserved19[0x83];
    struct CommandQueue *writing;
    struct CommandQueue *executing;
    unsigned char reserveda4[0xb4];
    int counter;
    unsigned char reserved15c[4];
    void (*callback)(int event);
};



extern void func_00190878(void);

void func_00110548(void)
{
    struct CommandQueue *queue = bss_0049ecf0.writing;
    if (!queue || bss_0049ecf0.pending)
        return;
    bss_0049ecf0.executing = queue;
    bss_0049ecf0.enabled = 1;
    if (queue->execution_threshold < queue->count) {
        bss_0049ecf0.pending = 1;
        if (queue->mode == 0x300) {
            func_0012fb38();
            func_00190878();
            func_0012fb48();
        }
    }
}
