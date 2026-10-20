#define NW_RELEASE
#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/text_writer.h"

void func_001ab540(ScreenResourceManager *scene)
{
    scene->vtable = &data_0022359c;
    if (!func_00177124(7)) {
        func_00177084();
        func_00181b58(3);
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->index);
    }
    func_00174480(scene);
}
