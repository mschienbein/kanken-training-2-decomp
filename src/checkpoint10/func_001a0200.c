#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/selection.h"
#include "game/screen.h"

void func_001a0200(struct W24BMenuState *screen)
{
    if (func_00177124(7) == 0) {
        func_00177084();
        func_00181b58(2);
        func_001c0f5c(screen->selection_slots);
        func_001c1cfc(screen->footer_slots);
        func_00176e24(&screen->base);
        func_00176df4();
        func_00176dc8(screen->base.index);
    }
}
