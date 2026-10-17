#include "game/functions.h"
/* Array-element constructor callback: the shared ARM EABI vector constructor
 * contract returns the element pointer. The return-only machine body preserves
 * that pointer even though this caller ignores it.
 * [INFERENCE] Source-level constructor identity, not a void callback ABI. */
void *func_001c0e44(void *element)
{
    return element;
}
