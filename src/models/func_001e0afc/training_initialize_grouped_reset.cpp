/* [INFERENCE] Split helper translation unit: with same-TU definitions ARMCC
 * removes the page accessor and erased lifecycle boundaries before linking.
 * Separating their natural definitions tests armlink inlining without noinline,
 * address escapes or retention directives. Training-screen initialization unit. Address adjacency
 * does not prove historical TU membership. The adjacent lesson scene has a
 * different layout and supplies no shared static needed by this initializer. */
#include "game/training_screen.h"



static void clear_control_input(SummaryTrainingScene *screen, int index);
static void hide_alternate_item(SummaryTrainingScene *screen, int index);

void SummaryTrainingScene::activate()
{
    SummaryTrainingScene *screen = this;
    if (func_0019efd0())
        return;
    func_00176d98();
    screen->elapsed = 0.0f;
    screen->selected = -1;
    screen->page = 0;
    screen->count = 0;
    screen->item = 0;
    screen->action = 0;
    screen->selected_part = -1;
    screen->kind = -1;
    screen->changed = 0;
    screen->scroll_shown = 0;
    screen->display_mode = 0;
    screen->progress = 0.0f;
    screen->score = 100;
    screen->flag1bf = 0;
    screen->ready = 0;
    screen->repeat = 0;
    screen->choices[0] = 0;
    screen->choices[1] = 0;
    screen->choices[2] = 0;
    screen->choices[3] = 0;
    screen->choice_count = 0;
    screen->attempts = 0;
    screen->first = 1;
    screen->identifier = -1;
    screen->free_mode = screen->mode != 1;
    /* [INFERENCE] This accessor and the prompt/optional lifecycle identities
     * below were erased by armlink; supplied definitions do not recover their
     * historical identities or prove original outlining. */
    screen->page_count = TU924_page_count(data_0023ecf4);
    if (screen->mode == 1) {
        func_001dc170(0);
        func_001dc230(0);
    }
    func_00174454(11, 0)->count = 0;
    func_001dbc84();
    func_001e3050(screen);
    if (screen->mode == 0 || screen->mode == 1 || screen->mode == 2)
        screen->control_index = 0;
    else if (screen->mode == 3)
        screen->control_index = 1;
    switch (func_00173620()) {
    case 0: screen->duration = 40; break;
    case 1: screen->duration = 60; break;
    case 2: screen->duration = 10; break;
    case 3: screen->duration = 15; break;
    case 4: screen->duration = 20; break;
    }
    TU924_reset_prompts(screen->prompts);
    if (screen->message) {
        func_00176c6c(screen->message);
        func_0017a43c(screen->message, reinterpret_cast<const unsigned short *>(L"\x3000"),
                     reinterpret_cast<const unsigned short *>(L"\x3000"), 0);
        func_00170db4(screen->message, 1);
    }
    if (screen->primary_list) {
        func_00176b24(screen->primary_list);
        func_00176ae8(screen->primary_list, 4, 8);
    }
    if (screen->navigation) {
        func_0017697c(screen->navigation);
        func_001795cc(screen->navigation, 7, -1);
        hide_alternate_item(screen, 8);
        hide_alternate_item(screen, 9);
        hide_alternate_item(screen, 13);
        hide_alternate_item(screen, 14);
    }
    if (screen->scalar)
        func_00172b38(screen->scalar);
    if (screen->controller68)
        TU924_reset_optional(screen->controller68);
    if (screen->animation) {
        func_0017673c(screen->animation);
        func_0017a258(screen->animation, 100.0f, 100.0f);
    }
    if (screen->indicators) {
        switch (screen->mode) {
        case 0:
        case 1:
        case 2:
            func_001762ac(screen->indicators, 0);
            break;
        case 3:
            func_001762ac(screen->indicators, 3);
            break;
        }
        if (screen->mode != 0)
            func_0016e1c8(screen->indicators, 5, 1);
        if (screen->animation)
            func_0017a164(screen->indicators, func_0017a210(screen->animation));
    }
    if (screen->characters)
        func_00175dcc(&screen->characters->controller);
    if (screen->controller80)
        func_001b4aa0(screen->controller80);
    if (screen->labels) {
        func_00175d90(screen->labels);
        func_00175d18(screen->labels, bss_00291230);
        if (screen->mode == 0)
            func_0017a034(screen->labels, 0, screen->duration);
    }
    if (screen->style) {
        func_00175b8c(&screen->style->base);
        func_00175ab8(&screen->style->base, 0, 79, 79, 79, 255);
        func_00175ab8(&screen->style->base, 1, 79, 79, 79, 255);
        func_00175ab8(&screen->style->base, 2, 79, 79, 79, 255);
        func_00175ab8(&screen->style->base, 3, 79, 79, 79, 255);
        func_00175ab8(&screen->style->base, 4, 79, 79, 79, 255);
        func_00175ab8(&screen->style->base, 5, 79, 79, 79, 255);
        func_00175ab8(&screen->style->base, 6, 79, 79, 79, 255);
        func_00175ab8(&screen->style->base, 7, 79, 79, 79, 255);
        func_00179160(&screen->style->base, 4, 0.4f, 0.45f);
        func_00179160(&screen->style->base, 5, 0.4f, 0.45f);
        func_00179160(&screen->style->base, 6, 0.4f, 0.45f);
        func_00179160(&screen->style->base, 7, 0.4f, 0.45f);
    }
    if (screen->controls) {
        func_001e9944(screen->controls);
        switch (screen->mode) {
        case 0:
        case 1:
        case 2:
            func_00179600(screen->controls, 0, 1);
            func_00179600(screen->controls, 1, 0);
            break;
        case 3:
            func_00179600(screen->controls, 0, 0);
            func_00179600(screen->controls, 1, 1);
            break;
        }
        func_00179600(screen->controls, 2, 1);
        func_00179600(screen->controls, 3, 0);
        func_00179600(screen->controls, 4, 0);
        func_00179600(screen->controls, 5, 0);
        func_00179600(screen->controls, 6, 0);
        func_00179600(screen->controls, 7, 0);
        func_00179600(screen->controls, 8, 0);
        func_00179d48(screen->controls, screen->control_index, 1, 0);
        func_00177c58(screen, 0);
        clear_control_input(screen, 4);
        clear_control_input(screen, 5);
        clear_control_input(screen, 6);
        clear_control_input(screen, 7);
        func_001770cc(screen->controls, screen->control_index, 0);
        func_001770cc(screen->controls, 2, 0);
        func_001770cc(screen->controls, 8, 0);
        func_00175a90(screen->controls, 8, 1);
        func_00175a5c(screen->controls, 8, 70.0f, -28.0f, 184.0f, 184.0f);
    }
    if (screen->options)
        func_001758e4(screen->options);
    switch (func_001758d4()) {
    case 0: func_0017abf8(0, 0x1000003); break;
    case 1: func_0017abf8(0, 0x1000004); break;
    case 2: func_0017abf8(0, 0x1000005); break;
    }
    /* [INFERENCE] The descriptor supplies the member identity; this object's
     * relationship to the canonical opaque MenuScene is unrecovered. */
    func_00177b10(screen, data_00218718);
    func_00175848();
}

static void clear_control_input(SummaryTrainingScene *screen, int index)
{
    func_0016f930(screen->controls, index);
}

static void hide_alternate_item(SummaryTrainingScene *screen, int index)
{
    func_00170e50(screen->navigation, index);
}

