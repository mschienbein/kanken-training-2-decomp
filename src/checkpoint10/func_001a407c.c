#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"








void func_001a407c(struct ScreenResourceManager *state)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_00181b58(4);
        func_00176e24(state);
        func_00176df4();
        func_00176dc8(state->index);
    }
}
