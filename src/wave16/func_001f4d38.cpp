#include "game/indexed_slots.h"

/* Slot zero is reserved; user slots begin with element one. */
/* [INFERENCE] The pointer-sized slot payload is represented as an opaque
 * pointer; no direct caller distinguishes a pointer from an integer word. */

void *func_001f4d38(IndexedSlotObject *object, unsigned index, void *value)
{
    if (object->slots != 0) {
        void *previous = object->slots[index + 1];
        object->slots[index + 1] = value;
        return previous;
    }
    return 0;
}
