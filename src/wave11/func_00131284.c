#include "game/stream_manager.h"


void func_00131284(struct StreamManager *manager)
{
    func_00183f18(&manager->requests);
    func_00183f18(&manager->completions);
    manager->initialized = 0;
}
