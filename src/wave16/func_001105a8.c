#include "game/bss_0049ecf0.h"




void func_001105a8(CommandQueueCompletionCallback callback)
{
    struct CommandQueue *queue =
        bss_0049ecf0.writing;
    if (queue != 0)
        queue->callback = callback;
}
