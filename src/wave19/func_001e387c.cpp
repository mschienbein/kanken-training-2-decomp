#include "game/functions.h"
#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/controller_transitions.h"

/* [INFERENCE] Virtual member names are unrecovered; the callback uses slot 4. */

void IntroScene::activate()
{
    IntroScene *state = this;
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
    state->remaining = 0.0f;
    state->sound = -1;
    state->page = 0;
    state->question = 0;
    func_001b6c90(state->first_options);
    func_001b7e9c(state->second_options);
    if (state->primary_list != 0)
        func_00176b24(state->primary_list);
    if (state->navigation != 0)
        func_0017697c(state->navigation);
    if (func_00170234(0) > 0)
        func_0017433c(0, (int)func_0017adb4() * 15);
    func_00177b10(state, &MenuScene::update);
    func_00175848();
}
