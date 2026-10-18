#include "game/functions.h"
#include "game/bss_0049ecf0.h"


void func_0011db78(int count)
{
    struct CommandQueue *queue = bss_0049ecf0.writing;
    if (queue) {
        if (count == -1)
            queue->complete = 1;
        else
            queue->commands[count - 1].reserved01 = 1;
    }
}
