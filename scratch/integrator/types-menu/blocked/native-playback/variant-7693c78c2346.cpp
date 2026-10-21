#include "game/deferred_callback.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/input.h"
#include "game/runtime.h"
#include "game/playback_screen.h"

/* [INFERENCE] Virtual slot names; byte offsets are fixed by the callback and tail dispatch. */

void ScreenInitializationState::update()
{
    ScreenInitializationState *state = this;
    switch (state->mode) {
    case 0: {
        const L043PlaybackEntry *entry = &data_0023f068[state->count];
        func_0017a6f4(state->primary_list, entry->selection);
        func_00168118(state->first_options, entry->first);
        func_001795cc(state->navigation, entry->alternate, -1);
        func_00167f7c(state->second_options, entry->second);
        state->elapsed = entry->seconds * 1000.0f;
        func_0017abf8(0, 0x01000000);
        func_00181b68(7, 0, 1);
        state->mode = 1;
        break;
    }
    case 1:
        if (!func_00177124(7)) {
            unsigned buttons = bss_004972d8.buttons;
            if (buttons & 0x204000) {
                state->index = 2;
                func_001e9818(state->controls, 0, 0, 1);
                state->selected = func_0015e31c(-1u, 0x01000016);
                state->mode = 2;
            }
        }
        break;
    case 2:
        if (!func_0015e2b4(state->selected)) {
            state->selected = -1;
            func_00181b68(7, 1, 1);
            state->mode = 3;
        }
        break;
    case 3:
        if (!func_00177124(7)) {
            ++state->count;
            if (state->count >= 1) {
                func_00177b10(state, data_002188e0);
                return;
            }
            state->mode = 0;
        }
        break;
    }
    func_001b6d44(state->first_options);
    func_001b7f50(state->second_options);
    if (state->style && !func_0016ec14(&state->playback_motion.mode)) {
        func_00175ab8(&state->style->base, 0, 79, 79, 79,
                      (unsigned char)func_0016e874(&state->playback_motion));
        func_0016e924(&state->playback_motion, -1.0f);
    }
    state->finish();
}
