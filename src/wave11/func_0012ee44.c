#include "game/exp_heap.h"
int func_0012ee44(struct ExpHeap *heap, int direction)
{
    struct ExpHeapData *data = &heap->data;
    int previous = data->direction;
    data->direction = direction;
    return previous;
}
