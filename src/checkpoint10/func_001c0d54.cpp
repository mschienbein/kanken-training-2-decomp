#define NW_RELEASE
#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/text_writer.h"
#include "game/runtime.h"


static __forceinline void release_completion_scene(ReviewScreenState *scene)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_001c3854(scene->slots19);
        func_001c42e8(scene->slots18);
        if (scene->first_asset)
            func_00183208(scene->first_asset);
        if (scene->second_asset)
            func_00183208(scene->second_asset);
        func_0016f024(&scene->motion);
        if (scene->option) {
            func_0016d4f8(scene->resources.controller);
            func_0016d310(scene->resources.controller);
        }
        if (scene->option) {
            func_0016f55c(3, 1, 1u, 0u, 0u, 0.0f, 0.0f);
            scene->resources.index = 10;
        } else {
            scene->resources.index = 4;
        }
        func_00176e24(&scene->resources);
        func_00176df4();
        func_00176dc8(scene->resources.index);
    }
}

void func_001c0d54(ReviewScreenState *scene)
{
    scene->resources.vtable = &data_0022374c;
    release_completion_scene(scene);
    func_00174480(scene);
}
