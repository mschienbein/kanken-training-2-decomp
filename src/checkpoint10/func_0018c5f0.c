#include "game/bss_0049ecf0.h"


extern void func_00190878(void);

void func_0018c5f0(void *destination, const void *source, unsigned size)
{
    struct CommandQueue *queue = bss_0049ecf0.writing;
    struct Command *command = &queue->commands[queue->count];
    command->kind = 0;
    command->source = source;
    /* Kind zero uses the established generic payload words as address/count. */
    command->size = (unsigned)destination;
    command->flags = size;
    func_0012fb38();
    ++queue->count;
    if (bss_0049ecf0.executing == queue && bss_0049ecf0.enabled && !bss_0049ecf0.pending) {
        bss_0049ecf0.pending = 1;
        func_00190878();
    }
    func_0012fb48();
}
