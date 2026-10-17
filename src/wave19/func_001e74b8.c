#include "game/random.h"
#include "game/input.h"
#include "game/selection.h"

#include "game/w11_control.h"

extern void func_001b3c0c(struct AnimationState *slider, int delta);


void func_001e74b8(struct W6SelectionScreen *screen)
{
    struct AnimationState *slider = func_00167284();
    if (slider) {
        if (bss_004972d8.direction & 0x800)
            func_001b3c0c(slider, 1);
        else if (bss_004972d8.direction & 0x400)
            func_001b3c0c(slider, -1);
    }
    if (!screen->list || !(bss_004972d8.buttons & 0x200000))
        return;
    if (func_0018ffac(2)) {
        func_00179600(screen->list, 0, 0);
        func_00179600(screen->list, 1, 1);
    } else {
        func_00179600(screen->list, 0, 1);
        func_00179600(screen->list, 1, 0);
    }
}
