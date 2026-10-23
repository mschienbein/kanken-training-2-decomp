#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/notification.h"
#include "game/screen.h"

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
struct QueuedDisplayHandle {
    BoundDisplayObject *object;
    unsigned int flags;
};
struct QueuedDisplayState {
    unsigned char reserved00[0x28];
    QueuedDisplayHandle *handle;
};
struct AnimatedDepartureState : CallbackTarget {
    unsigned char reserved04[0x48];
    AnimationOwner *animations;
    unsigned char reserved50[0x2c];
    QueuedDisplayState *display;
};

/* INTEGRATOR: move to include/game/transition.h */
extern "C" {
void func_0017433c(int index, int ticks);
void func_00181b68(unsigned mask, unsigned direction, int animate);
void func_001793e4(QueuedDisplayState *state, int active);
void func_001dd638(AnimatedDepartureState *screen);
}

void func_001dd638(AnimatedDepartureState *screen)
{
    func_00172d48(-1);
    func_001793e4(screen->display, 0);
    func_001770cc(screen->animations, 0, 0);
    func_001770cc(screen->animations, 1, 0);
    func_001770cc(screen->animations, 2, 0);
    func_0017433c(0, (int)func_0017adb4() * 60);
    func_00181b68(7, 1, 1);
    func_00177b10(screen, &CallbackTarget::complete_transition);
}
