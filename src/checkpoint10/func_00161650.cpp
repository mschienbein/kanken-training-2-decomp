#include "game/screen.h"
#include "game/selection.h"

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
struct TrainingMenuTransient;
struct TrainingMenuScene : MenuSceneBase {
    unsigned char reserved90[0x20];
    int selection;
    unsigned char reservedb4[0x20];
    int overlay_active;
    unsigned char reservedd8[0x2b0];
    TrainingMenuTransient *transient;
};

/* INTEGRATOR: move to include/game/menu_scene.h */
extern "C" {
int func_00177124(int channel);
void func_00176e24(MenuSceneBase *scene);
void func_00176dc8(int scene);
void func_00174480(void *object);
void func_00179a48(TrainingMenuScene *scene, int active);
void func_00161650(TrainingMenuScene *scene);
}

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
        case 0: scene->next_scene = 7; break;
        case 1: scene->next_scene = 9; break;
        case 2: scene->next_scene = 8; break;
        case 3: scene->next_scene = 10; break;
        }
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->next_scene);
    }
}
