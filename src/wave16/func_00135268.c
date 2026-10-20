#include "game/functions.h"
#include "game/resource_runtime.h"
#include "game/exp_heap.h"


void func_00135268(unsigned heap, int capacity)
{
    struct DisplayObject *entry;
    int i;
    struct ResourceEventPool *const pool = &data_00242238;

    pool->count = capacity;
    entry = func_0019d798(heap, 4, capacity * sizeof(*entry));
    pool->active_count = 0;
    i = 0;
    pool->events = entry;
    if (i < pool->count) {
        do {
            func_0018663c(entry);
            ++i;
            ++entry;
        } while (i < pool->count);
    }
}
