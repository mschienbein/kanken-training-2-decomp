#include "game/command_state.h"
#include "game/runtime.h"

extern void *func_00100c80(void *elements, void *(*destroy)(void *element));

struct ArrayOwner *func_001748b8(struct ArrayOwner *owner)
{
    if (owner->elements != 0) {
        func_001ef888(func_00100c80(owner->elements, func_001ef880));
        owner->elements = 0;
    }
    return owner;
}
