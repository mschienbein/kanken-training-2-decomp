#include "game/exp_heap.h"
unsigned short func_0012ee34(struct ExpHeap *heap, int group)
{
    struct ExpHeapData *data = &heap->data;
    unsigned short previous = data->group;
    data->group = group;
    return previous;
}
