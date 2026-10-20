#include "game/playback_screen.h"
#include "game/deferred_callback.h"
#include "game/selection.h"
#include "game/screen.h"
#include "game/runtime.h"

void ScreenInitializationState::activate()
{
    ScreenInitializationState *state = this;
    if (func_0019efd0())
        return;
    func_00176d98();
    state->elapsed = 0.0f;
    state->mode = 0;
    state->selected = -1;
    state->count = 0;
    func_001b6c90(state->first_options);
    func_001b7e9c(state->second_options);
    if (state->primary_list)
        func_00176b24(state->primary_list);
    if (state->navigation)
        func_0017697c(state->navigation);
    if (state->controls) {
        func_001e9944(state->controls);
        func_00179600(state->controls, 0, 1);
    }
    if (state->controller88)
        func_001adabc(state->controller88);
    if (state->style) {
        struct MotionKey keys[4];
        func_00175b8c(&state->style->base);
        func_00179160(&state->style->base, 0, 0.45f, 0.55f);
        func_00175ab8(&state->style->base, 0, 79, 79, 79, 0);
        func_00166eac(&state->style->base, 0, 134.0f, -96.0f);
        func_00192720(keys, data_002188e8, sizeof(keys));
        func_0016ee38(&state->playback_motion);
        if (func_0016ece8(&state->playback_motion, keys, 1)) {
            func_0016ec84(&state->playback_motion, 0, -1);
            func_0016ec6c(&state->playback_motion, 0);
            func_0016ec28(&state->playback_motion, -1);
        }
    }
    /* [INFERENCE] The opaque callback receiver is this state object; the
     * member-function identity is retained in its named original descriptor. */
    func_00177b10(state, data_002188d8);
    func_00175848();
}
