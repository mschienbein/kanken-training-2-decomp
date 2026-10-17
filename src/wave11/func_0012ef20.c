#include "game/exp_heap.h"
int func_0012ef20(struct ExpHeap *heap, int mode)
{
    struct ExpHeapData *data = &heap->data;
    int previous = data->mode;
    data->mode = mode;
    return previous;
}
