#include "game/bss_0049ecf0.h"



extern void func_00186ef0(void);

void func_0011e8a4(void)
{
    struct CommandQueue *queue = bss_0049ecf0.executing;
    struct Command *command = &queue->commands[queue->cursor];
    if (!(command->flags & 2))
        queue->committed_bytes = command->size + queue->committed_bytes;
    func_00186ef0();
}
