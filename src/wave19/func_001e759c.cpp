#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/selection.h"
#include "game/controller_transitions.h"

/* [INFERENCE] Virtual member names are unrecovered; the callback uses slot 4. */

extern "C" void func_001e759c(DetailedTransitionScreenW19G *state)
{
    if (func_0019efd0() == 0) {
        Screen *selector;
        AnimationState *other;
        func_00176d98();
        state->phase = -1;
        state->field9c = 0;
        if (state->buttons != 0) {
            func_00176b24(state->buttons);
            func_0017a6f4(state->buttons, 4);
        }
        selector = func_001671d4();
        if (selector != 0) {
            func_0017697c(selector);
            func_001795cc(selector, 7, -1);
        }
        other = func_00167284();
        if (other != 0) {
            func_0017673c(other);
            func_0017a258(other, 100.0f, 60.0f);
        }
        if (state->region != 0) {
            func_00175d90(state->region);
            func_0016f9a8(state->region, 0, 0.7f, 0.7f);
            func_0016f9a8(state->region, 1, 0.6f, 0.6f);
            func_0016f9a8(state->region, 2, 0.6f, 0.6f);
            func_0017a034(state->region, 0, 60);
            func_0017a034(state->region, 1, 5);
            func_0017a034(state->region, 2, 10);
        }
        if (state->choices != 0) {
            func_001e9944(state->choices);
            func_00179600(state->choices, 0, 1);
            func_00179600(state->choices, 1, 0);
        }
        if (func_00170234(0) > 0)
            func_0017433c(0, (int)func_0017adb4() * 15);
        func_00177b10(state, &CallbackTarget::callback4);
        func_00175848();
    }
}
