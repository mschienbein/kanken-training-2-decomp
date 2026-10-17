#include "game/stream_manager.h"

struct StreamQueue *func_00183f18(struct StreamQueue *queue)
{
    queue->mutex.depth = -1;
    return queue;
}
