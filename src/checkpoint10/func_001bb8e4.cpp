#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/text_writer.h"
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
struct SceneFeedback;
struct MenuPageA;
struct MenuPageB;
struct SceneDisplayResource;
struct FeedbackMenuScene : MenuSceneBase {
    unsigned char reserved90[0x14];
    SceneFeedback *feedback;
    unsigned char reserveda8[0x18c];
    MenuPageA *first_page;
    MenuPageB *second_page;
    SceneDisplayResource *display;
};

/* INTEGRATOR: move to include/game/menu_scene.h */
extern "C" {










void func_001c3c50(SceneDisplayResource *display);


extern const unsigned data_002236b8[];
void func_001bb8e4(FeedbackMenuScene *scene);
}

static __forceinline void release_feedback_scene(FeedbackMenuScene *scene)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_001c31c0(scene->first_page);
        func_001c3618(scene->second_page);
        if (scene->display)
            func_001c3c50(scene->display);
        func_001dc0a4(scene->feedback);
        if (func_0016d000(-1)) {
            scene->next_scene = 13;
            func_00181b58(9);
        } else {
            func_00181b58(8);
            func_0016f55c(scene->next_scene = 3, 0, 1, 0, 0.0f, 0.0f, 0);
        }
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->next_scene);
    }
}

void func_001bb8e4(FeedbackMenuScene *scene)
{
    scene->vtable = data_002236b8;
    release_feedback_scene(scene);
    func_00174480(scene);
}
