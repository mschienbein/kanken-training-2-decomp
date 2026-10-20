#include "game/functions.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/runtime.h"

#include "game/resource_runtime.h"



void SceneAt0028dd7c::depart()
{
    SceneAt0028dd7c *screen = this;
    if (func_00177124(7))
        return;
    func_00177084();
    func_001c3854(screen->slots19);
    func_001c42e8(screen->slots18);
    if (screen->first_asset.asset)
        func_00183208(screen->first_asset.asset);
    if (screen->second_asset.asset)
        func_00183208(screen->second_asset.asset);
    func_0016f024(&screen->motion);
    if (screen->option) {
        func_0016d4f8(screen->drawing);
        func_0016d310(screen->drawing);
    }
    if (screen->option) {
        func_0016f55c(3, 1, 1, 0, 0, 0.0f, 0.0f);
        screen->index = 10;
    } else {
        screen->index = 4;
    }
    func_00176e24(screen);
    func_00176df4();
    func_00176dc8(screen->index);
}
