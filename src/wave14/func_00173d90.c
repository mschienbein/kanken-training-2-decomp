#include "game/display_animation.h"
#include "game/screen.h"


void func_00173d90(struct DisplayRegistration *const *slots, int index, int active)
{
    if (slots[index] != 0 && slots[index]->enabled != 0)
        func_001c1e6c(slots, index, active);
}
