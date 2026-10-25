#include "game/display_target.h"

/* The array runtime passes opaque element storage; this empty lifecycle
 * callback preserves it without asserting a DisplayTarget object model. */
void *func_00110358(void *storage)
{
    return storage;
}
