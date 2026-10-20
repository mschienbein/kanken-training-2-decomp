#include "game/selection.h"
#include "game/screen.h"
#include "game/animation.h"

struct L044TrainingFrame;
struct L044StatisticsPanel;
struct L044ProgressPanel;
struct L044CategoryState {
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
    void *other_panel;
    struct Screen *category_panel;
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
    _Bool show_category;
    unsigned char reserved155[3];
    int previous_modes[2];
    int previous_levels[2];
    int previous_values[2];
    int target_value;
    int level_capacity;
    int total_value;
    struct L044TrainingFrame *frame;
};

extern void func_001667e8(struct L044TrainingFrame *frame, int active, int count);
extern int func_00166778(int category);
extern void func_00166f1c(void *screen, int index, int enabled);
extern void func_0016f888(struct W6HandlePanel10 *panel);
extern void func_00174078(struct L044StatisticsPanel *panel);
extern void func_00179d48(struct AnimationOwner *owner, int panel_index, int element_index, int enabled);
extern void func_001aef6c(struct L044ProgressPanel *panel, int level);
extern void func_001af00c(struct L044ProgressPanel *panel, int level, int value, int capacity);
extern void func_001af154(struct L044ProgressPanel *panel, int value, int capacity);
extern void func_001af258(struct L044ProgressPanel *panel, int active);
extern void func_001c1190(struct L044TrainingFrame *frame, int enabled, int category);
extern void func_001db9b0(int which, int *mode, int *level, int *value);
extern int func_001dbd60(int level);
extern int func_001dbd7c(void);
extern void func_001ed2cc(struct L044CategoryState *screen);
extern void func_001ed05c(struct L044CategoryState *screen);

void func_001ec1b8(struct L044CategoryState *screen, int reset)
{
    int i;
    _Bool active;
    if (reset) {
        func_001795cc(screen->navigation, 11, -1);
        func_001667e8(screen->frame, 0, 12);
    }
    func_00178e98(screen->choices, 1);
    func_00178e98(screen->choices, 2);
    func_00178e98(screen->choices, 3);
    func_00178e98(screen->choices, 4);
    func_00178e98(screen->choices, 5);
    switch (screen->previous_states[0]) {
    case 5:
        func_00170e50(screen->navigation, 12);
        func_00179600(screen->choices, 0, 0);
        func_00179600(screen->choices, 1, 0);
        break;
    case 6:
        func_00179600(screen->choices, 2, 0);
        func_00166f1c(screen->numbers, 12, 0);
        func_00166f1c(screen->numbers, 13, 0);
        func_00166f1c(screen->numbers, 14, 0);
        func_00166f1c(screen->numbers, 15, 0);
        func_00166f1c(screen->numbers, 16, 0);
        func_00166f1c(screen->numbers, 17, 0);
        func_00166f1c(screen->numbers, 18, 0);
        func_00166f1c(screen->numbers, 19, 0);
        func_00166f1c(screen->numbers, 20, 0);
        func_00166f1c(screen->numbers, 21, 0);
        func_00166f1c(screen->numbers, 22, 0);
        func_00166f1c(screen->numbers, 23, 0);
        func_001667e8(screen->frame, 0, 12);
        break;
    case 7:
        func_00179600(screen->choices, 3, 0);
        func_00174078(screen->statistics);
        func_00166f1c(screen->numbers, 5, 0);
        func_00166f1c(screen->numbers, 6, 0);
        break;
    case 8:
        func_00179600(screen->choices, 3, 0);
        func_00166f1c(screen->numbers, 5, 0);
        func_00166f1c(screen->numbers, 6, 0);
        func_00173ef4(screen->category_panel);
        func_001c1190(screen->frame, 0, 0);
        for (i = 30; i <= 59; ++i)
            func_00166f1c(screen->numbers, i, 0);
        break;
    case 3:
        func_00173ef4(screen->category_panel);
        func_00166f1c(screen->numbers, 7, 0);
        func_00166f1c(screen->numbers, 8, 0);
        func_00166f1c(screen->numbers, 9, 0);
        func_00166f1c(screen->numbers, 10, 0);
        func_00166f1c(screen->numbers, 11, 0);
        func_001af258(screen->progress, 0);
        func_00179600(screen->choices, 4, 0);
        break;
    }
    active = 1;
    switch (screen->states[0]) {
    case 5:
        func_0017a6f4(screen->buttons, 11);
        func_00174210(screen->selector, 110);
        func_00173d8c(screen->handle_panel, 29);
        func_001795cc(screen->navigation, 11, 12);
        func_00179600(screen->choices, 0, 1);
        func_00179600(screen->choices, 1, 1);
        func_00178e98(screen->choices, 0);
        goto show_category;
    case 6:
        func_0017a6f4(screen->buttons, 11);
        func_00174210(screen->selector, 114);
        func_00173d8c(screen->handle_panel, 29);
        func_001795cc(screen->navigation, 11, -1);
        func_00170e50(screen->navigation, 12);
        func_00179600(screen->choices, 2, 1);
        switch (screen->mode) {
        case 1:
            func_00179d48((struct AnimationOwner *)screen->choices, 2, 0, 0);
            break;
        case 0:
            func_00179d48((struct AnimationOwner *)screen->choices, 2, 1, 0);
            break;
        }
    show_category:
        func_0017a034(screen->numbers, 0, func_00174470());
        func_0017a034(screen->numbers, 4, func_00166778(-1));
        func_0017a0e0(screen->display, 0, 1);
        screen->show_category = active;
        return;
    case 7:
        func_00179600(screen->choices, 3, 1);
        func_0017a034(screen->numbers, 4, func_00166778(-1));
        func_001ed2cc(screen);
        return;
    case 8:
        func_00179600(screen->choices, 3, 1);
        func_0017a034(screen->numbers, 4, func_00166778(-1));
        func_001b472c(screen->category_panel, 8);
        func_001ed05c(screen);
        return;
    case 1:
        {
            int remaining;
            func_001db9b0(0, &screen->previous_modes[0], &screen->previous_levels[0],
                           &screen->previous_values[0]);
            func_001db9b0(1, &screen->previous_modes[1], &screen->previous_levels[1],
                           &screen->previous_values[1]);
            screen->level_capacity = func_001dbd60(screen->previous_levels[1]);
            screen->total_value = func_001dbd7c() + screen->previous_values[1];
            if (screen->previous_levels[0] == screen->previous_levels[1] &&
                screen->previous_modes[0] == screen->previous_modes[1])
                screen->target_value = screen->previous_values[0];
            else
                screen->target_value = screen->level_capacity;
            func_0017a9ac(screen->buttons);
            func_00174210(screen->selector, -1);
            func_0016f888(screen->handle_panel);
            func_00170e50(screen->navigation, 12);
            if (screen->previous_modes[1] == 1)
                func_001b472c(screen->category_panel, 9);
            else if (screen->previous_modes[1] < 11)
                func_001b472c(screen->category_panel, 10);
            else
                func_001b472c(screen->category_panel, 11);
            func_001aef6c(screen->progress, screen->previous_levels[1]);
            func_001af00c(screen->progress, screen->previous_levels[1],
                          screen->previous_values[1], screen->level_capacity);
            func_001af154(screen->progress, screen->previous_values[1], screen->level_capacity);
            func_0017a034(screen->numbers, 7, screen->level_capacity - screen->previous_values[1]);
            remaining = screen->total_value - screen->previous_values[1];
            if (remaining > 999)
                remaining = 999;
            func_0017a034(screen->numbers, 8, remaining);
            func_0017a034(screen->numbers, 9, screen->previous_levels[1] + 1);
            func_0017a034(screen->numbers, 10, screen->previous_levels[1]);
            if ((unsigned)(screen->previous_modes[1] - 2) < 9)
                func_0017a034(screen->numbers, 11, screen->previous_modes[1]);
            func_00166f1c(screen->numbers, 0, 0);
            func_00166f1c(screen->numbers, 1, 0);
            func_00166f1c(screen->numbers, 2, 0);
            func_00166f1c(screen->numbers, 3, 0);
            func_00166f1c(screen->numbers, 4, 0);
            func_0017a0e0(screen->display, 0, 0);
            screen->show_category = 0;
        }
        return;
    }
}
