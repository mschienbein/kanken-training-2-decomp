#include "game/stream_manager.h"
void func_0019124c(struct StreamManager *arena, unsigned serial)
{
    for (;;) {
        struct StreamEvent *head;
        struct StreamEvent *command;
        struct StreamEvent *next;
        unsigned completed;
        command = func_001829e0(&arena->completions);
        /* ORIGINAL BUG: a null dequeue result is read before its null check. */
        completed = command->serial;
        arena->completed_serial = completed;
        head = command;
        if (command) {
            do {
                next = command->next;
                if (next)
                    command = next;
            } while (next);
            arena->read_index = command->start;
            arena->full = 0;
            if (arena->write_index == arena->read_index) {
                arena->read_index = 0;
                arena->write_index = 0;
            }
        }
        if (head->serial == serial)
            break;
    }
}
