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
struct EffectsMenuScene : MenuSceneBase {
    unsigned char reserved90[0xf0];
    SceneDisplayResource *display;
    unsigned char reserved184[0x20];
    MotionState motion;
};

/* INTEGRATOR: move to include/game/menu_scene.h */
extern "C" {
int func_00177124(int channel);
void func_00177084(void);
void func_00181b58(int scene);
void func_00176e24(MenuSceneBase *scene);
void func_00176df4(void);
void func_00176dc8(int scene);
void func_00174480(void *object);
void func_0016d4f8(MenuScenePanel *panel);
void func_0016d310(MenuScenePanel *panel);
void func_00183208(SceneDisplayResource *display);
void func_0016f024(MotionState *motion);
void func_001e440c(EffectsMenuScene *scene);
}

void func_001e440c(EffectsMenuScene *scene)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_00181b58(10);
        if (scene->panel) {
            func_0016d4f8(scene->panel);
            func_0016d310(scene->panel);
        }
        if (scene->display)
            func_00183208(scene->display);
        func_0016f024(&scene->motion);
        /* ORIGINAL BUG: panel cleanup is repeated unconditionally, including for null. */
        func_0016d310(scene->panel);
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->next_scene);
    }
}
