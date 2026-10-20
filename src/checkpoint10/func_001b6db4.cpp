#define NW_RELEASE
#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/prompt.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/text_writer.h"
#include "game/runtime.h"
#include "game/screen.h"


extern "C" void func_001b6db4(ScreenResourceManager *screen)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_001c1600(func_0016e270());
        func_00176e24(screen);
        func_00176df4();
        func_00176dc8(screen->index);
    }
}
