#define NW_RELEASE
#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/selection_cleanup.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/text_writer.h"
#include "game/runtime.h"
#include "game/screen.h"


extern "C" void func_001e3ec8(GridCleanupScreen *screen)
{
    screen->base.vtable = &data_002237b4;
    if (!func_00177124(7)) {
        func_00177084();
        func_00168618(screen->six_handles);
        func_001685c0(screen->four_handles);
        if (screen->base.component05)
            func_00168454(screen->base.component05);
        if (screen->base.component08)
            func_001682e8(screen->base.component08);
        func_00176e24(&screen->base);
        func_00176df4();
        func_00176dc8(screen->base.index);
    }
    func_00174480(screen);
}
