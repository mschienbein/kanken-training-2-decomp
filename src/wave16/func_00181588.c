#include "game/screen.h"

extern struct DisplayObject *func_0013566c(struct DisplayObject *object, int index,
                                        int capacity, int count, int heap, void *storage);

struct DisplayObject *func_00181588(struct DisplayObject *object, int index,
                                   int capacity, int count)
{
    return func_0013566c(object, index, capacity, count, 1, 0);
}
