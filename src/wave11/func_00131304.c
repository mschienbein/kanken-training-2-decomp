#include "game/audio_tasks.h"
void func_00131304(struct ObjectManager *manager)
{
    if (manager->running) {
        manager->stopping = 1;
        func_00148210();
        manager->running = 0;
    }
}
