#include "game/deferred_callback.h"

/* [INFERENCE] Vtable slot names: complete/update/leave are at +0x14/+0x18/+0x20. */
class CallbackTarget {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void complete_transition();
    virtual void update_transition();
    virtual void slot7();
    virtual void leave_transition();
};
struct SceneDepartureState : CallbackTarget {
    unsigned char reserved04[0x8c];
    int state;
};

/* INTEGRATOR: move to include/game/transition.h */
extern "C" {
void func_00181b68(unsigned mask, unsigned direction, int animate);
void func_001e7bec(SceneDepartureState *screen);
}

void func_001e7bec(SceneDepartureState *screen)
{
    screen->update_transition();
    if (screen->state != -1) {
        func_00181b68(7, 1, 1);
        func_00177b10(screen, &CallbackTarget::complete_transition);
    } else {
        screen->leave_transition();
    }
}
