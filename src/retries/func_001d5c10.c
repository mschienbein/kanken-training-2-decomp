#include "game/object_manager.h"
#include "game/functions.h"



void func_001d5c10(struct Operation *operation)
{
    unsigned char linked = operation->linked;
    operation->stopping = 1;
    if (linked) {
        func_0017e634(func_00185d1c(), &operation->manager_entry);
        operation->linked = 0;
    }
    if (operation->running)
        operation->running = 0;
}
