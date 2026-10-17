#include "game/runtime.h"

struct WidgetActionState;
struct MenuOptionGroup;
struct ScreenActionGroup;
struct SelectionList;
struct AlternateSelectionList;

struct AnimationKey {
    float time;
    unsigned flags;
    unsigned char reserved08[0x10];
};
struct KeyAnimation {
    int mode;
    unsigned key_flags;
    unsigned flags;
    float time;
    float speed;
    float first_time;
    float last_time;
    int first;
    int last;
    int current;
    float blend;
    int count;
    struct AnimationKey *keys;
};
struct ScreenInitializationState {
    unsigned char reserved00[0x4c];
    struct ScreenActionGroup *actions;
    void *screen;
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
    struct KeyAnimation animation;
};
struct ScreenInitializationOptions {
    int first;
    int second;
};

extern const struct AnimationKey data_002188e8[4];
extern const struct ScreenInitializationOptions data_002188d8;
extern int func_0019efd0(void);
extern void func_00176d98(void);
extern void func_001b6c90(struct MenuOptionGroup *options);
extern void func_001b7e9c(struct MenuOptionGroup *options);
extern void func_00176b24(struct SelectionList *selection);
extern void func_0017697c(struct AlternateSelectionList *selection);
extern void func_001e9944(struct ScreenActionGroup *actions);
extern void func_00179600(struct ScreenActionGroup *actions, int index, int active);
extern void func_001adabc(struct WidgetActionState *state);
extern void func_00175b8c(void *screen);
extern void func_00179160(void *screen, int index, float x, float y);
extern void func_00175ab8(void *screen, int index,
                         int red, int green, int blue, int alpha);
extern void func_00166eac(void *screen, int index, float x, float y);
extern void func_00192720(void *destination, const void *source, unsigned size);
extern void func_0016ee38(struct MotionState *state);
extern int func_0016ece8(struct KeyAnimation *animation,
                        const struct AnimationKey *keys, int heap);
extern void func_0016ec84(struct KeyAnimation *animation, int first, int last);
extern void func_0016ec6c(struct KeyAnimation *animation, int looping);
extern void func_0016ec28(struct KeyAnimation *animation, int frame);
extern void func_00177b10(struct ScreenInitializationState *state,
                         int first, int second);
extern void func_00175848(void);

void func_001e807c(struct ScreenInitializationState *state)
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
        struct AnimationKey keys[4];
        func_00175b8c(state->screen);
        func_00179160(state->screen, 0, 0.45f, 0.55f);
        func_00175ab8(state->screen, 0, 79, 79, 79, 0);
        func_00166eac(state->screen, 0, 134.0f, -96.0f);
        func_00192720(keys, data_002188e8, sizeof(keys));
        /* INTEGRATOR: KeyAnimation refines game/runtime.h MotionState's padding;
         * both have speed at 0x10, count at 0x2c and allocation at 0x30. */
        func_0016ee38((struct MotionState *)&state->animation);
        if (func_0016ece8(&state->animation, keys, 1)) {
            func_0016ec84(&state->animation, 0, -1);
            func_0016ec6c(&state->animation, 0);
            func_0016ec28(&state->animation, -1);
        }
    }
    func_00177b10(state, data_002188d8.first, data_002188d8.second);
    func_00175848();
}
