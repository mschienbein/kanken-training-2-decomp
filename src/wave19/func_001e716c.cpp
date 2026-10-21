#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/controller_transitions.h"

/* [INFERENCE] Virtual member names are unrecovered; the callback uses slot 4. */

void DepartureScene::activate()
{
    DepartureScene *state = this;
    if (func_0019efd0() == 0) {
        func_00176d98();
        state->phase = 0;
        state->progress = 0.0f;
        state->previous_phase = -1;
        if (state->primary_list != 0) {
            func_00176b24(state->primary_list);
            func_0017a6f4(state->primary_list, 0);
        }
        if (state->navigation != 0) {
            func_0017697c(state->navigation);
            func_001795cc(state->navigation, 0, -1);
        }
        if (func_00170234(0) > 0)
            func_0017433c(0, (int)func_0017adb4() * 15);
        func_00177b10(state, &MenuScene::update);
        func_00175848();
    }
}
