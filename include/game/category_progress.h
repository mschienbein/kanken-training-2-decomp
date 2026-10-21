#ifndef GAME_CATEGORY_PROGRESS_H
#define GAME_CATEGORY_PROGRESS_H

#include "game/selection.h"
#include "game/screen.h"
#include "game/animation.h"
#include "game/glyph_style.h"
#include "game/deferred_callback.h"
#include "game/screen_operations.h"

struct L044TrainingFrame;
struct WidgetActionState;
struct RecordSelection4;
#ifdef __cplusplus
struct CategoryScene : MenuScene {
    virtual void build();
    virtual ~CategoryScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001ef790 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
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
#endif
#ifdef __cplusplus
extern "C" {
extern const SceneMemberCallback data_00218860;
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern void func_00166ae8(struct CategoryScene *screen, int state, int transition);
extern void func_001c144c(struct L044TrainingFrame *frame);
void func_001c1590(struct L044TrainingFrame *frame);
void func_001dbf40(unsigned *value, unsigned *minutes, unsigned *seconds, unsigned *milliseconds);
extern void func_00170298(struct RecordSelection4 *panel);
extern void func_001ae2e4(struct WidgetActionState *panel);
extern void func_001ec1b8(struct CategoryScene *screen, int reset);
extern int func_00172994(int channel);
extern int func_0017abf8(int channel, int sound);
#ifdef __cplusplus
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
