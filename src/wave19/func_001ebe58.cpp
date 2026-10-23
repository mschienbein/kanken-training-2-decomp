#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/controller_transitions.h"

/* [INFERENCE] Virtual member names are unrecovered; the callback uses slot 4. */

extern "C" void func_001ebe58(TransitionScreenW19G *state)
{
    if (func_0019efd0() == 0) {
        func_00176d98();
        state->phase = 0;
        state->progress = 0.0f;
        state->previous_phase = -1;
        if (state->buttons != 0) {
            func_00176b24(state->buttons);
            func_0017a6f4(state->buttons, 0);
        }
        if (state->selector != 0) {
            func_0017697c(state->selector);
            func_001795cc(state->selector, 0, -1);
        }
        if (func_00170234(0) > 0)
            func_0017433c(0, (int)func_0017adb4() * 15);
        func_00177b10(state, &CallbackTarget::callback4);
        func_00175848();
    }
}
