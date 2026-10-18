#include "game/selection.h"

/* [INFERENCE] Partial layout of the selection-menu scene. The three nested
 * phase slots precede the selection state observed by the neighbouring TU. */
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
int func_00177124(unsigned int mask);
void func_00181b68(unsigned int mask, int active, int animate);
void func_001a3978(MenuPhaseScene *scene);
void func_001a3ee0(MenuPhaseScene *scene);
void func_001a2fc4(W6SelectionScreen *scene, int entered);
void func_001a2d9c(MenuPhaseScene *scene);
}

/* [INFERENCE] File-local phase-stack accessors are shared by the two
 * transition arms; the historical helper names and TU boundary are unknown. */
static int current_phase(const MenuPhaseScene *scene);
static void set_current_phase(MenuPhaseScene *scene, int phase);

static void update_transition(MenuPhaseScene *scene)
{
    switch (current_phase(scene)) {
    case 10:
        func_00181b68(7, 1, 1);
        set_current_phase(scene, 11);
        break;
    case 11:
        if (func_00177124(7))
            return;
        scene->selection = -1;
        func_001a2fc4(reinterpret_cast<W6SelectionScreen *>(scene), 0);
        func_00181b68(7, 0, 1);
        set_current_phase(scene, 12);
        break;
    case 12: {
        /* [INFERENCE] A zero poll result is also the idle-mode token. */
        const int pending = func_00177124(7);
        if (!pending)
            scene->mode = pending;
        return;
    }
    default:
        return;
    }
}

void func_001a2d9c(MenuPhaseScene *scene)
{
    switch (scene->mode) {
    case 0: func_001a3978(scene); break;
    case 1: update_transition(scene); break;
    case 2: func_001a3ee0(scene); break;
    }
}

static int current_phase(const MenuPhaseScene *scene)
{
    return scene->phases[scene->mode];
}

static void set_current_phase(MenuPhaseScene *scene, int phase)
{
    scene->phases[scene->mode] = phase;
}

