#include "game/menu_controller.h"

/* [INFERENCE] Shared text/group accessors and the state-stack setter model
 * repeated operations. Historical TU membership and helper names are unknown. */
static int child_text(const struct AnimationOwner *owner, int animation, int child);
static struct TransitionController *transition_group(const struct MenuControllerState *screen);
static void set_current_stage(struct MenuControllerState *screen, int stage);

void func_001a2814(struct MenuControllerState *state)
{
    if (func_00171e30()) {
        state->before_update();
        func_001c1080(state->member);
        /* [INFERENCE] armlink-inlined secondary-member update was removed.
         * The receiver load at the erased site proves +0xe4, not its identity. */
        tu441_update_secondary(state->secondary_member);
        func_0016337c(state, 1);
        state->after_update();
        if (state->animations && func_00174324(state->animations, 2)) {
            float x, y;
            for (int i = 0; i < 12; ++i) {
                func_00179030(state->animations, 2, i, &x, &y);
                func_00171db8(state->animations, 3, i, x, y);
                func_00171db8(state->animations, 4, i, x + 25.0f, y + 40.0f);
                func_00171db8(state->animations, 5, i, x, y - 41.0f);
                func_001ea6d8(state->labels, i + 4, x + 34.0f, y - 41.0f);
                int text = child_text(state->animations, 2, i);
                if (child_text(state->animations, 3, i) != text)
                    func_00171d40(state->animations, 3, i, text);
                if (child_text(state->animations, 4, i) != text)
                    func_00171d40(state->animations, 4, i, text);
                if (child_text(state->animations, 5, i) != text)
                    func_00171d40(state->animations, 5, i, text);
            }
        }
    }
}

void func_001a3ee0(struct MenuControllerState *screen)
{
    switch (screen->stages[screen->depth]) {
    case 13:
        func_00172c04(transition_group(screen), 0);
        func_001770cc(screen->animations, 14, 0);
        func_001770cc(screen->animations, 1, 0);
        func_001770cc(screen->animations, 2, 0);
        func_001770cc(screen->animations, 3, 0);
        func_001770cc(screen->animations, 4, 0);
        func_001770cc(screen->animations, 5, 0);
        func_001770cc(screen->animations, 6, 0);
        func_001770cc(screen->animations, 7, 0);
        func_001770cc(screen->animations, 8, 0);
        func_001770cc(screen->animations, 9, 0);
        func_001770cc(screen->animations, 10, 0);
        func_001770cc(screen->animations, 11, 0);
        func_001770cc(screen->animations, 12, 0);
        func_001770cc(screen->animations, 13, 0);
        set_current_stage(screen, 14);
        break;
    case 14:
        /* [INFERENCE] The result/receiver contract is evidenced, but the
         * identity of this armlink-inlined, removed accessor is unknown. */
        if (tu441_transition_active(transition_group(screen)) == 0) {
            func_00172c28(transition_group(screen));
            func_001770cc(screen->animations, 14, 1);
            switch (screen->stages[0]) {
            case 2:
                func_001770cc(screen->animations, 1, 1);
                break;
            case 3:
                /* Preserve the shared declaration: the established callee
                 * ignores this forwarded receiver. Its historical class is unknown. */
                func_001770cc(screen->animations,
                    func_00171c7c(reinterpret_cast<struct W6SelectionScreen *>(screen)), 1);
                break;
            }
            screen->depth = 0;
        }
        break;
    }
}

static int child_text(const struct AnimationOwner *owner, int animation, int child)
{
    return func_00171d88(owner, animation, child);
}

static struct TransitionController *transition_group(const struct MenuControllerState *screen)
{
    return screen->group;
}

static void set_current_stage(struct MenuControllerState *screen, int stage)
{
    screen->stages[screen->depth] = stage;
}
