#include "game/region_heap.h"

int func_00125a5c(struct HeapStorage *context)
{
    if (!func_0013f110(context->heap, context->pending.count))
        return -1;
    if (func_001856d8(context))
        return context->pending.count - 1;
    func_0013f06c(context->heap, 0);
    return -1;
}
