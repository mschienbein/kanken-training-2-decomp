/* [INFERENCE] Partial menu-scene layout and descriptive field names. */
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
struct MenuPhaseScene : MenuSceneBase {
    int mode;
    int phases[3];
    unsigned char reserveda0[0x10];
    int selection;
};

/* INTEGRATOR: move to include/game/menu_scene.h */
extern "C" {
int func_00177124(int channel_mask);
void func_00181b68(int channel_mask, int active, int animate);
void func_001a7b54(MenuPhaseScene *scene);
void func_001a8048(MenuPhaseScene *scene);
void func_00171268(MenuPhaseScene *scene, int selection);
void func_001a77d4(MenuPhaseScene *scene);
}

static void set_phase(MenuPhaseScene *scene, int phase)
{
    scene->phases[scene->mode] = phase;
}

static void update_transition(MenuPhaseScene *scene)
{
    switch (scene->phases[scene->mode]) {
    case 10:
        func_00181b68(7, 1, 1);
        set_phase(scene, 11);
        break;
    case 11:
        if (func_00177124(7))
            return;
        scene->selection = -1;
        func_00171268(scene, 0);
        func_00181b68(7, 0, 1);
        set_phase(scene, 12);
        break;
    case 12: {
        int busy = func_00177124(7);
        /* A completed query supplies the idle value for the scene mode. */
        if (busy == 0)
            scene->mode = busy;
        break;
    }
    }
}

void func_001a77d4(MenuPhaseScene *scene)
{
    switch (scene->mode) {
    case 0: func_001a7b54(scene); break;
    case 1: update_transition(scene); break;
    case 2: func_001a8048(scene); break;
    }
}
