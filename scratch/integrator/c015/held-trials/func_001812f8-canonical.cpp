#include "game/audio_tasks.h"

/* Destroy the two real queue members without recovering their owner from a byte offset. */
static void destroy_queue(struct StreamQueue **queue)
{
    func_00183f18(*queue);
    *queue = func_00181328(*queue);
}
void func_001812f8(struct StreamManager *manager)
{
    struct StreamQueue *queue = &manager->completions;
    destroy_queue(&queue);
    queue = &manager->requests;
    destroy_queue(&queue);
}
