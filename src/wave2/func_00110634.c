#include "game/bss_0049ecf0.h"

struct Command {
    unsigned char kind;
    unsigned char reserved01;
    unsigned char ready;
    unsigned char reserved03;
    const void *source;
    unsigned size;
    unsigned flags;
    unsigned char reserved10[12];
};

struct CommandQueue {
    unsigned char reserved00[0x18];
    struct Command *commands;
    unsigned reserved1c;
    int count;
    int completed_count;
};

extern void func_0012fb38(void);
extern void func_0012fb48(void);

void func_00110634(void)
{
    struct CommandQueue *queue = bss_0049ecf0.executing;
    if (queue) {
        func_0012fb38();
        if (bss_0049ecf0.pending) {
            queue->commands[queue->completed_count - 1].ready = 1;
            *((unsigned char *)&bss_0049ecf0 + 0x18) = 1;
        } else {
            bss_0049ecf0.enabled = 0;
        }
        func_0012fb48();
    }
}
