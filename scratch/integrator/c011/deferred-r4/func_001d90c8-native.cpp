#include "game/reusable_pool.h"

void ReusablePoolR41306::release(ReusableObjectR41306 *object)
{
    /* ORIGINAL BUG: the owner write precedes the pool's null-pointer check. */
    object->owner = 0;
    pool.Free(object);
}
