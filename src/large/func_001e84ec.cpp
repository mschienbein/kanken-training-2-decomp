#include "game/deferred_callback.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/input.h"
#include "game/runtime.h"
#include <string.h>
#include "game/playback_screen.h"

/* [INFERENCE] Virtual slot names; byte offsets are fixed by the callback and tail dispatch. */

extern "C" void func_001e84ec(struct ScreenInitializationState *state)
{
    switch (state->mode) {
    case 0: {
        const L043PlaybackEntry *entry = &data_0023f068[state->entry];
        func_0017a6f4(state->selection, entry->selection);
        func_00168118(state->first_options, entry->first);
        func_001795cc(state->alternate, entry->alternate, -1);
        func_00167f7c(state->second_options, entry->second);
        state->milliseconds = entry->seconds * 1000.0f;
        func_0017abf8(0, 0x01000000);
        func_00181b68(7, 0, 1);
        state->mode = 1;
        break;
    }
    case 1:
        if (!func_00177124(7)) {
            unsigned buttons;
            /* The canonical input layout leaves this button word opaque. */
            memcpy(&buttons, bss_004972d8.reserved10, sizeof(buttons));
            if (buttons & 0x204000) {
                state->transition = 2;
                func_001e9818(state->actions, 0, 0, 1);
                state->handle = func_0015e31c(-1u, 0x01000016);
                state->mode = 2;
            }
        }
        break;
    case 2:
        if (!func_0015e2b4(state->handle)) {
            state->handle = -1;
            func_00181b68(7, 1, 1);
            state->mode = 3;
        }
        break;
    case 3:
        if (!func_00177124(7)) {
            ++state->entry;
            if (state->entry >= 1) {
                func_00177b10(state, data_002188e0);
                return;
            }
            state->mode = 0;
        }
        break;
    }
    func_001b6d44(state->first_options);
    func_001b7f50(state->second_options);
    if (state->screen && !func_0016ec14(&state->animation.mode)) {
        unsigned char alpha = (unsigned char)func_0016e874(&state->animation);
        func_00175ab8(state->screen, 0, 79, 79, 79, alpha);
        func_0016e924(&state->animation, -1.0f);
    }
    state->slot08();
}
