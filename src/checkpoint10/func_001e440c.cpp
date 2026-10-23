#define NW_RELEASE
#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/text_writer.h"
#include "game/runtime.h"


void func_001e440c(EffectsMenuScene *scene)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_00181b58(10);
        if (scene->base.controller) {
            func_0016d4f8(scene->base.controller);
            func_0016d310(scene->base.controller);
        }
        if (scene->display)
            func_00183208(scene->display);
        func_0016f024(&scene->motion);
        /* ORIGINAL BUG: input cleanup is repeated unconditionally, including for null. */
        func_0016d310(scene->base.controller);
        func_00176e24(&scene->base);
        func_00176df4();
        func_00176dc8(scene->base.index);
    }
}
