#include "game/audio_tasks.h"
void func_00183f08(struct AudioTaskManager *manager)
{
    manager->lock.depth = -1;
    func_00183f18(&manager->queue);
}
