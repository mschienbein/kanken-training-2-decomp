#include "game/functions.h"
#include "game/bss_0049ecf0.h"

struct ExtendedCommandQueue {
    struct CommandQueue queue;
    int cursor;
    unsigned capacity;
    unsigned captured_size;
    struct CommandPatch *patch;
    unsigned reserved38;
    unsigned char complete;
    unsigned char flush;
};

void func_0011db78(int count)
{
    struct ExtendedCommandQueue *queue = (struct ExtendedCommandQueue *)bss_0049ecf0.writing;
    if (queue) {
        if (count == -1)
            queue->complete = 1;
        else
            queue->queue.commands[count - 1].reserved01 = 1;
    }
}
