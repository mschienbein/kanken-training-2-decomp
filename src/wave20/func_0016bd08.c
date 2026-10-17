#include "game/bss_0049ecf0.h"

extern void func_00190878(void);

void func_0016bd08(const void *source, void *destination, unsigned size)
{
    struct CommandSystem *system = &bss_0049ecf0;
    struct CommandQueue *queue = system->writing;
    struct Command *command = &queue->commands[queue->count];
    command->kind = 0;
    command->source = source;
    /* Kind zero uses the generic payload words as destination and byte count. */
    command->size = (unsigned)destination;
    command->flags = size;
    func_0012fb38();
    ++queue->count;
    if (system->executing == queue && system->enabled && !system->pending) {
        system->pending = 1;
        func_00190878();
    }
    func_0012fb48();
}
