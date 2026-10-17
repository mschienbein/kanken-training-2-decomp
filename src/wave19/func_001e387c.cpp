#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/controller_transitions.h"

/* [INFERENCE] Virtual member names are unrecovered; the callback uses slot 4. */

extern "C" void func_001e387c(RestartTransitionScreenW19G *state)
{
    if (func_0019efd0() != 0)
        return;
    func_00176d98();
    if (data_0023f028 != 0) {
        data_0023f028 = 0;
        func_00181e44();
        state->phase = 1;
    } else {
        state->phase = 0;
    }
    state->progress = 0.0f;
    state->previous_phase = -1;
    state->field9c = 0;
    state->fielda0 = 0;
    func_001b6c90(state->first);
    func_001b7e9c(state->second);
    if (state->buttons != 0)
        func_00176b24(state->buttons);
    if (state->selector != 0)
        func_0017697c(state->selector);
    if (func_00170234(0) > 0)
        func_0017433c(0, (int)func_0017adb4() * 15);
    func_00177b10(state, &CallbackTarget::callback4);
    func_00175848();
}
