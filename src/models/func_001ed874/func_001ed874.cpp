#include "game/category_progress.h"





static inline void initialize_overlay(struct CategoryScreen *screen, int layer, int state)
{
    screen->active_layer = layer;
    screen->states[layer] = state;
    screen->phase = 0;
}

void func_001ed874(struct CategoryScreen *screen)
{
    if (func_0019efd0())
        return;
    func_00176d98();
    initialize_overlay(screen, 1, 10);
    switch (screen->mode) {
    case 1: func_00166ae8(screen, 6, 0); break;
    case 0: func_00166ae8(screen, 5, 0); break;
    case 2: func_00166ae8(screen, 1, 0); break;
    }
    screen->previous_states[0] = -1;
    screen->previous_states[1] = -1;
    screen->elapsed = 0.0f;
    screen->transition = -1;
    screen->category = func_00174470();
    screen->previous_category = -1;
    screen->count_phase = 0;
    screen->previous_modes[0] = 0;
    screen->previous_modes[1] = 0;
    screen->previous_levels[0] = 0;
    screen->previous_levels[1] = 0;
    screen->previous_values[0] = 0;
    screen->previous_values[1] = 0;
    screen->target_value = 0;
    screen->level_capacity = 0;
    screen->total_value = 0;
    if (screen->category == -1)
        screen->category = 11;
    screen->selected_category = screen->category;
    func_001c144c(screen->frame);
    if (screen->buttons)
        func_00176b24(screen->buttons);
    /* [INFERENCE] callee identity: inlined by armlink; these receiver-only
     * no-op initializers use distinct typed source definitions. Their original
     * callee identities are not observable at the rewritten call sites. */
    if (screen->selector)
        initialize_selector(screen->selector);
    if (screen->handle_panel)
        initialize_handle_panel(screen->handle_panel);
    if (screen->progress)
        func_00170298(screen->progress);
    if (screen->navigation)
        func_0017697c(screen->navigation);
    if (screen->other_panel) {
        func_00172b38(screen->other_panel);
        func_00172a14(screen->other_panel, 4);
    }
    if (screen->category_panel)
        initialize_category_panel(screen->category_panel);
    if (screen->statistics)
        func_001ae2e4(screen->statistics);
    if (screen->numbers) {
        func_00175d90(screen->numbers);
        func_00175d18(screen->numbers, bss_00291244);
    }
    if (screen->display) {
        func_00175b8c(screen->display);
        func_00179160(screen->display, 0, 0.8f, 1.0f);
        func_00175ab8(screen->display, 0, 79, 79, 79, 255);
        func_00179160(screen->display, 1, 0.5f, 0.5f);
        func_00175ab8(screen->display, 1, 79, 79, 79, 255);
        func_00179160(screen->display, 2, 0.5f, 0.5f);
        func_00175ab8(screen->display, 2, 79, 79, 79, 255);
        func_00179160(screen->display, 3, 0.5f, 0.5f);
        func_00175ab8(screen->display, 3, 79, 79, 79, 255);
    }
    if (screen->choices) {
        func_001e9944(screen->choices);
        func_00179600(screen->choices, 0, 0);
        func_00179600(screen->choices, 1, 0);
        func_00179600(screen->choices, 2, 0);
        func_00179600(screen->choices, 3, 0);
        func_00179600(screen->choices, 4, 0);
        func_00179600(screen->choices, 5, 0);
    }
    func_001ec1b8(screen, 1);
    if (func_00172994(0) != 0x1000002)
        func_0017abf8(0, 0x1000002);
    /* [INFERENCE] The callback receiver is this state; its member identity is
     * retained by the original, typed member-function descriptor. */
    func_00177b10(reinterpret_cast<CallbackTarget *>(screen), data_00218860);
    func_00175848();
}
