#include "game/bss_0049ecf0.h"

void func_00110634(void)
{
    struct CommandQueue *queue = bss_0049ecf0.executing;
    if (queue) {
        func_0012fb38();
        if (bss_0049ecf0.pending) {
            queue->commands[queue->completed_count - 1].ready = 1;
            bss_0049ecf0.ready = 1;
        } else {
            bss_0049ecf0.enabled = 0;
        }
        func_0012fb48();
    }
}
