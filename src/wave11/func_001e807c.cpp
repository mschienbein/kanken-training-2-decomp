#include "game/deferred_callback.h"
#include "game/selection.h"
#include "game/screen.h"
#include "game/runtime.h"


struct ScreenInitializationState {
    unsigned char reserved00[0x4c];
    struct W6ListController *actions;
    struct W8DisplaySet *screen;
    struct SelectionList *selection;
    unsigned char reserved58[8];
    struct AlternateSelectionList *alternate;
    unsigned char reserved64[0x24];
    struct WidgetActionState *action;
    unsigned reserved8c;
    int mode;
    float elapsed;
    int selected;
    int count;
    struct MenuOptionGroup *first_options;
    struct MenuOptionGroup *second_options;
    struct MotionState animation;
};


extern "C" void func_001e807c(struct ScreenInitializationState *state)
{
    if (func_0019efd0())
        return;
    func_00176d98();
    state->elapsed = 0.0f;
    state->mode = 0;
    state->selected = -1;
    state->count = 0;
    func_001b6c90(state->first_options);
    func_001b7e9c(state->second_options);
    if (state->selection)
        func_00176b24(state->selection);
    if (state->alternate)
        func_0017697c(state->alternate);
    if (state->actions) {
        func_001e9944(state->actions);
        func_00179600(state->actions, 0, 1);
    }
    if (state->action)
        func_001adabc(state->action);
    if (state->screen) {
        struct MotionKey keys[4];
        func_00175b8c(state->screen);
        func_00179160(state->screen, 0, 0.45f, 0.55f);
        func_00175ab8(state->screen, 0, 79, 79, 79, 0);
        func_00166eac(state->screen, 0, 134.0f, -96.0f);
        func_00192720(keys, data_002188e8, sizeof(keys));
        func_0016ee38(&state->animation);
        if (func_0016ece8(&state->animation, keys, 1)) {
            func_0016ec84(&state->animation, 0, -1);
            func_0016ec6c(&state->animation, 0);
            func_0016ec28(&state->animation, -1);
        }
    }
    /* [INFERENCE] The opaque callback receiver is this state object; the
     * member-function identity is retained in its named original descriptor. */
    func_00177b10(reinterpret_cast<CallbackTarget *>(state), data_002188d8);
    func_00175848();
}
