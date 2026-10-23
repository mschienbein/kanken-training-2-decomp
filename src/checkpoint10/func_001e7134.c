#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"







void func_001e7134(struct ScreenResourceManager *state)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_00176e24(state);
        func_00176df4();
        func_00176dc8(state->index);
    }
}
