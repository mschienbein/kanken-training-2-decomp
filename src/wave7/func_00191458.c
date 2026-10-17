#include "game/stream_manager.h"

unsigned int func_00191458(struct StreamManager *manager, struct StreamEvent *event)
{
    if (manager->pending_tail == 0)
        manager->pending_head = event;
    else
        manager->pending_tail->next = event;
    manager->pending_tail = event;
    event->next = 0;
    return manager->serial;
}
