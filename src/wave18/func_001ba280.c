#include "game/functions.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/runtime.h"

#include "game/resource_runtime.h"




void func_001ba280(struct ReviewScreenState *screen)
{
    if (func_00177124(7))
        return;
    func_00177084();
    func_001c31c0(screen->slots3);
    func_001c3618(screen->slots5);
    if (screen->geometry)
        func_001c3c50(screen->geometry);
    func_001dc0a4(screen->index);
    if (func_0016d000(-1)) {
        screen->resources.index = 13;
        func_00181b58(9);
    } else {
        func_00181b58(8);
        screen->resources.index = 3;
        func_0016f55c(3, 0, 1, 0, 0, 0.0f, 0.0f);
    }
    func_00176e24(&screen->resources);
    func_00176df4();
    func_00176dc8(screen->resources.index);
}
