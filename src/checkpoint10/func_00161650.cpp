#define NW_RELEASE
#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/text_writer.h"
#include "game/screen.h"
#include "game/selection.h"


static void close_overlay(TrainingMenuScene *scene)
{
    if (scene->overlay_active) {
        func_00179a48(scene, 0);
        scene->overlay_active = 0;
    }
}

void func_00161650(TrainingMenuScene *scene)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_00181b58(7);
        close_overlay(scene);
        /* [INFERENCE] callee identity: inlined by armlink; transient-object cleanup. */
        func_00174480(scene->transient);
        switch (scene->selection) {
        case 0: scene->base.index = 7; break;
        case 1: scene->base.index = 9; break;
        case 2: scene->base.index = 8; break;
        case 3: scene->base.index = 10; break;
        }
        func_00176e24(&scene->base);
        func_00176df4();
        func_00176dc8(scene->base.index);
    }
}
