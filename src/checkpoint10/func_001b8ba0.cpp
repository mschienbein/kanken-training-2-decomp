#define NW_RELEASE
#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/text_writer.h"

void func_001b8ba0(PagedMenuSceneA *scene)
{
    if (!func_00177124(7)) {
        func_00177084();
        scene->base.index = 4;
        func_0016d394(scene->rows);
        if (scene->base.controller) {
            func_0016d4f8(scene->base.controller);
            func_0016d310(scene->base.controller);
        }
        func_0017731c(scene->base.secondary);
        func_00176e24(&scene->base);
        func_00176df4();
        func_00176dc8(scene->base.index);
    }
}
