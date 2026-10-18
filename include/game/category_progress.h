#ifndef GAME_CATEGORY_PROGRESS_H
#define GAME_CATEGORY_PROGRESS_H

#include "game/selection.h"
#include "game/screen.h"
#include "game/animation.h"
#include "game/glyph_style.h"
#include "game/deferred_callback.h"
#include "game/screen_operations.h"

#ifdef __cplusplus
struct L044TrainingFrame;
struct L044StatisticsPanel;
struct L044ProgressPanel;
struct CategoryScreen {
    unsigned char reserved00[0x40];
    struct RecordSet *records;
    unsigned char reserved44[4];
    struct Screen *numbers;
    struct W6ListController *choices;
    struct W8DisplaySet *display;
    struct Screen *buttons;
    struct W6Selector *selector;
    struct W6HandlePanel10 *handle_panel;
    struct Screen *navigation;
    struct L041Dialog *other_panel;
    void *category_panel;
    unsigned char reserved6c[4];
    struct L044ProgressPanel *progress;
    unsigned char reserved74[0x14];
    struct L044StatisticsPanel *statistics;
    unsigned char reserved8c[4];
    int active_layer;
    int states[2];
    int previous_states[2];
    int phase;
    float elapsed;
    int transition;
    int mode;
    int category;
    int previous_category;
    int category_values[12];
    int count_phase;
    int attempted[12];
    int correct[12];
    int selected_category;
    bool show_category;
    unsigned char reserved155[3];
    int previous_modes[2];
    int previous_levels[2];
    int previous_values[2];
    int target_value;
    int level_capacity;
    int total_value;
    struct L044TrainingFrame *frame;
};
extern "C" {
extern const MemberCallback data_00218860;
extern void func_00166ae8(struct CategoryScreen *screen, int state, int transition);
extern void func_001c144c(struct L044TrainingFrame *frame);
extern void func_00170298(struct L044ProgressPanel *panel);
extern void func_001ae2e4(struct L044StatisticsPanel *panel);
extern void func_001ec1b8(struct CategoryScreen *screen, int reset);
extern int func_00172994(int channel);
extern int func_0017abf8(int channel, int sound);
void func_001ed874(struct CategoryScreen *screen);
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] Return-only candidate initializers have no recovered historical identities. */
void initialize_selector(struct W6Selector *selector);
void initialize_handle_panel(struct W6HandlePanel10 *panel);
void initialize_category_panel(void *panel);
#ifdef __cplusplus
}
#endif

#endif
