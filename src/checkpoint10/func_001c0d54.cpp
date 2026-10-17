#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/text_writer.h"
#include "game/runtime.h"

/* [INFERENCE] Recovered partial menu-scene layout; names describe observed roles. */
struct MenuScenePanel;
struct MenuChannelA;
struct MenuChannelB;
struct MenuSceneBase {
    const void *vtable;
    unsigned char reserved04[0x10];
    MenuScenePanel *panel;
    unsigned char reserved18[0x1c];
    int next_scene;
    MenuChannelA *channel_a;
    MenuChannelB *channel_b;
    unsigned char reserved40[0x50];
};
struct SceneDisplayResource;
struct MenuCompletionPageA;
struct MenuCompletionPageB;
struct CompletionMenuScene : MenuSceneBase {
    unsigned char reserved90[0x28];
    int completed;
    unsigned char reservedbc[0x38];
    SceneDisplayResource *first_display;
    unsigned char reservedf8[0xc0];
    SceneDisplayResource *second_display;
    unsigned char reserved1bc[0x20];
    MotionState motion;
    MenuCompletionPageA *first_page;
    MenuCompletionPageB *second_page;
};

/* INTEGRATOR: move to include/game/menu_scene.h */
extern "C" {














extern const unsigned data_0022374c[];
void func_001c0d54(CompletionMenuScene *scene);
}

static __forceinline void release_completion_scene(CompletionMenuScene *scene)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_001c3854(scene->first_page);
        func_001c42e8(scene->second_page);
        if (scene->first_display)
            func_00183208(scene->first_display);
        if (scene->second_display)
            func_00183208(scene->second_display);
        func_0016f024(&scene->motion);
        if (scene->completed) {
            func_0016d4f8(scene->panel);
            func_0016d310(scene->panel);
        }
        if (scene->completed) {
            func_0016f55c(3, 1, 1, 0, 0.0f, 0.0f, 0);
            scene->next_scene = 10;
        } else {
            scene->next_scene = 4;
        }
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->next_scene);
    }
}

void func_001c0d54(CompletionMenuScene *scene)
{
    scene->vtable = data_0022374c;
    release_completion_scene(scene);
    func_00174480(scene);
}
