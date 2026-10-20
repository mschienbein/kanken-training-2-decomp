#include "game/deferred_callback.h"
#include "game/input.h"

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
struct StagedDepartureState : CallbackTarget {
    unsigned char reserved04[0x30];
    int completion;
    unsigned char reserved38[0x58];
    int state;
};

/* INTEGRATOR: move to include/game/transition.h */
extern "C" {
void func_00181b68(unsigned mask, unsigned direction, int animate);
int func_00177124(unsigned mask);
void func_001ec074(StagedDepartureState *screen);
}

static __forceinline bool has_departure_input(const TouchDeviceState *input)
{
    return (input->reserved10[0] & 0x10) != 0;
}

void func_001ec074(StagedDepartureState *screen)
{
    switch (screen->state) {
    case 0:
        func_00181b68(7, 0, 1);
        screen->state = 1;
        break;
    case 1:
        if (func_00177124(7) == 0 && has_departure_input(&bss_004972d8))
            screen->state = 2;
        break;
    case 2:
        screen->completion = 1;
        func_00181b68(7, 1, 1);
        func_00177b10(screen, &CallbackTarget::complete_transition);
        return;
    }
    screen->leave_transition();
}
