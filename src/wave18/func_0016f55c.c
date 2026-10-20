#include "game/functions.h"
#include "game/notification.h"

/* Core and VFP parameter allocation is independent. The callers supply all five
 * core words together with x and y in the first two floating-point inputs. */
void func_0016f55c(int group, int index, unsigned value, unsigned count,
                   unsigned flags, float x, float y)
{
    struct TransformEntry *entry = &bss_00280af0.transforms[group][index];
    entry->reserved00 = value;
    entry->x = x;
    entry->y = y;
    entry->count = count;
    entry->flags = flags;
}
