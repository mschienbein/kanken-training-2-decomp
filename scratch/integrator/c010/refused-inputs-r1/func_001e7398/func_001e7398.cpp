#include "game/input.h"
#include "game/deferred_callback.h"

/* [INFERENCE] Virtual method names are unrecovered; offsets establish their order. */
class CallbackTarget {
public:
    virtual void stage0();
    virtual void stage1();
    virtual void stage2();
    virtual void stage3();
    virtual void on_transition();
    virtual void on_complete();
    virtual void stage6();
    virtual void stage7();
    virtual void update();
};

struct ScenePhaseState : CallbackTarget {
    unsigned char reserved04[0x30];
    int index;
    unsigned char reserved38[0x58];
    int phase;
};

/* INTEGRATOR: move to include/game/controller_transitions.h */
extern "C" {
extern void func_00181b68(int group, int mode, int enabled);
extern int func_00177124(int mode);
void func_001e7398(ScenePhaseState *scene);
}

void func_001e7398(ScenePhaseState *scene)
{
    switch (scene->phase) {
    case 0:
        func_00181b68(7, 0, 1);
        scene->phase = 1;
        break;
    case 1:
        if (!func_00177124(7) && (bss_004972d8.reserved10[0] & 0x10))
            scene->phase = 2;
        break;
    case 2:
        scene->index = 0x12;
        func_00181b68(7, 1, 1);
        func_00177b10(scene, &CallbackTarget::on_complete);
        return;
    }
    scene->update();
}
