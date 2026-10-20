#define NW_RELEASE
#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/text_writer.h"

static __forceinline void release_feedback_scene(ReviewScreenState *scene)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_001c31c0(scene->slots3);
        func_001c3618(scene->slots5);
        if (scene->geometry)
            func_001c3c50(scene->geometry);
        func_001dc0a4(scene->index);
        if (func_0016d000(-1)) {
            scene->resources.index = 13;
            func_00181b58(9);
        } else {
            func_00181b58(8);
            func_0016f55c(scene->resources.index = 3, 0, 1u, 0u, 0u, 0.0f, 0.0f);
        }
        func_00176e24(&scene->resources);
        func_00176df4();
        func_00176dc8(scene->resources.index);
    }
}

void func_001bb8e4(ReviewScreenState *scene)
{
    scene->resources.vtable = &data_002236b8;
    release_feedback_scene(scene);
    func_00174480(scene);
}
