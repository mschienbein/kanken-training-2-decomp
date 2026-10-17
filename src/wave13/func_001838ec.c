#include "game/audio_tasks.h"



/* The natural shutdown caller forwards func_00185de4() without adjustment. */
void func_001838ec(struct AudioTaskManager *manager)
{
    manager->active = 1;
    func_00183900(&manager->queue, 0);
}
