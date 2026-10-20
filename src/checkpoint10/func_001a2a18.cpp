#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/selection.h"
#include "game/text_writer.h"
#include "game/screen.h"


extern "C" void func_001a2a18(struct W24BMenuState *screen)
{
    screen->base.vtable = &data_00223544;
    if (func_00177124(7) == 0) {
        func_00177084();
        func_00181b58(2);
        func_001c0f5c(screen->selection_slots);
        func_001c1cfc(screen->footer_slots);
        func_00176e24(&screen->base);
        func_00176df4();
        func_00176dc8(screen->base.index);
    }
    func_00174480(screen);
}
