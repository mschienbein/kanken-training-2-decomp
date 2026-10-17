#include "game/screen.h"


void func_00195a08(struct DisplayObject *object, int index, int active)
{
    struct DisplayFlagSlot *slot = ((struct FlagDisplayObject *)object)->slots[index];
    if (active == 0) {
        slot->flags &= ~2u;
    } else if (active == 1) {
        slot->flags |= 2u;
    } else if (active == 2) {
        slot->flags ^= 2u;
    }
}
