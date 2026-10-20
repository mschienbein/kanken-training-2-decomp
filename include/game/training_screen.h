#ifndef GAME_TRAINING_SCREEN_H
#define GAME_TRAINING_SCREEN_H

#include "game/animation.h"
#include "game/input.h"
#include "game/selection.h"
#include "game/notification.h"
#include "game/deferred_callback.h"
#include "game/item_pages.h"
#include "game/messages.h"
#include "game/character_panel.h"
#include "game/screen_operations.h"
#include "game/transition_controller.h"

/* [INFERENCE] Separate receiver views, not a claim of common historical class identity. */
struct ColorPanel;
struct TextPanel;
struct FooterPanel;
struct TransitionController;
struct SelectionGrid {
    unsigned char reserved00[0x3c];
    int selected;
};
union TrainingControls {
    struct AnimationOwner *animation;
    struct W6ListController *list;
};
#ifdef __cplusplus
struct TrainingScreen : CallbackTarget {
#else
struct TrainingScreen {
    const void *vtable;
#endif
    unsigned char reserved04[0x10];
    struct W10DrawingInput *drawing;
    unsigned char reserved18[0x24];
    int resource;
    unsigned char reserved40[4];
    struct Screen *title;
    struct TextPanel *text;
    union TrainingControls controls;
    struct ColorPanel *colors;
    struct Screen *menu;
    struct W6Selector *selector;
    struct PassivePanel *auxiliary;
    struct Screen *information;
    struct FooterPanel *footer;
    struct PassivePanel *secondary_footer;
    struct AnimationState *navigation;
    unsigned char reserved70[8];
    struct MessageLayout *message;
    struct CharacterPanel *characters;
    unsigned char reserved80[0xc];
    struct TransitionController *transition;
    int layer;
    int previous_layer;
    int states[3];
    int initialized;
    float elapsed;
    int fieldac;
    int selected;
    int character_limit;
    int mode;
    int page;
    int last_page;
    int item_count;
    int item_index;
    unsigned char reservedcc[0x384];
    int drawing_state;
    unsigned short character;
    unsigned char reserved456[0x56];
    int alternate_drawing_state;
    unsigned short alternate_character;
    unsigned char reserved4b2[0x1e];
    struct SelectionGrid *grid;
};
#ifdef __cplusplus
extern "C" {
#endif
void func_001b8908(struct TrainingScreen *screen);
void func_001b9f18(struct TrainingScreen *screen);
#ifdef __cplusplus
extern const MemberCallback data_002184b0;
#endif
void func_0016d6dc(struct TrainingScreen *screen, int enabled);
void func_0016d600(struct TrainingScreen *screen, int mode, int enabled);
#ifdef __cplusplus
}
#endif
struct L041Dialog;
struct L041HandwritingPanel;
struct L041PromptTable;
struct L041Accessory;
struct R41169OptionalPanel;
union L041InitializeControls {
    struct AnimationOwner *animation;
    struct InputOwner *input;
    struct W6ListController *list;
};
struct L041InitializeScreen {
    unsigned char reserved00[0x44];
    struct L041Accessory *accessory;
    struct W11jVisualOwner *clock;
    union L041InitializeControls controls;
    struct W8DisplaySet *display;
    struct Screen *selection;
    unsigned char reserved58[8];
    struct Screen *alternate;
    struct L041Dialog *dialog;
    struct R41169OptionalPanel *optional_panel;
    struct AnimationState *scroll;
    unsigned char reserved70[8];
    struct MessageLayout *message;
    struct WidgetController *answer_panel;
    struct L041HandwritingPanel *handwriting;
    struct Screen *action_panel;
    unsigned char reserved88[8];
    int current, previous;
    int states[8];
    int phase;
    float elapsed;
    int selected;
    int mode, count, item, page_count, page;
    unsigned char reservedd8[0xc];
    int display_mode;
    unsigned reservede8;
    int scroll_shown;
    unsigned reservedf0;
    int duration;
    int action;
    int control_index;
    unsigned reserved100;
    int selected_part;
    unsigned reserved108;
    int kind;
    unsigned char changed;
    unsigned char reserved111[0xae];
    unsigned char flag1bf;
    unsigned char reserved1c0[0x208];
    float progress;
    int score;
    unsigned reserved3d0;
    struct L041PromptTable *prompts;
    unsigned char reserved3d8;
    unsigned char ready;
    unsigned char reserved3da[2];
    int repeat;
    unsigned char choices[4];
    int choice_count;
    int attempts;
    unsigned char first;
    bool free_mode;
    unsigned char reserved3ee[2];
    int identifier;
#ifdef __cplusplus
    void set_entry(unsigned index, unsigned value)
    {
        states[index] = value;
    }
#endif
};
#ifdef __cplusplus
extern "C" {
extern const MemberCallback data_00218718;
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001dc170(int value);
extern void func_001dc230(int value);
extern struct TransformEntry *func_00174454(int group, int index);
extern void func_001dbc84(void);
extern void func_001e3050(struct L041InitializeScreen *screen);
extern int func_00173620(void);
extern void func_00176c6c(struct MessageLayout *layout);
extern void func_00176ae8(struct Screen *screen, int first, int second);
extern void func_0017673c(struct AnimationState *state);
extern void func_001762ac(struct Screen *screen, int mode);
extern void func_0016e1c8(struct Screen *screen, int index, int enabled);
extern void func_0017a164(struct Screen *screen, int state);
extern void func_00175dcc(struct WidgetController *panel);
extern void func_001b4aa0(struct L041HandwritingPanel *panel);
extern void func_00179d48(struct AnimationOwner *owner, int panel_index, int element_index, int enabled);
extern void func_00177c58(struct L041InitializeScreen *screen, int selection);
extern void func_0016f930(const struct InputOwner *owner, int index);
extern void func_00175a90(struct InputOwner *owner, int index, int active);
extern void func_00175a5c(struct AnimationOwner *owner, int index,
                          float x, float y, float width, float height);
extern void func_001758e4(struct L041Accessory *accessory);
extern int func_0017abf8(int channel, int sound);
void func_001e0afc(struct L041InitializeScreen *screen);
#ifdef __cplusplus
}
#endif

struct L041PromptTable;
struct R41169OptionalPanel;
#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] This receiver is forwarded unchanged by 00168dd0; its +0x78
 * slot reaches the shared MessageLayout positioning and visibility family.
 * L041InitializeScreen is a partial view, not a complete historical class. */
void func_00169018(struct L041InitializeScreen *screen, int enabled);
void func_001690a8(struct L041InitializeScreen *screen, int enabled);
void func_001de2c0(struct L041InitializeScreen *screen);
void func_00168dd0(struct L041InitializeScreen *screen, int enabled);
/* The same +0x6c animation object is passed to positioning and hint control. */
void func_0016f174(struct AnimationState *state, int enabled);
void func_00179d48(struct AnimationOwner *owner, int panel_index,
                  int element_index, int enabled);
/* [INFERENCE] Erased accessor/lifecycle identities are not recovered. */
int TU924_page_count(const struct ItemPageSet *set);
void TU924_reset_prompts(struct L041PromptTable *table);
void TU924_reset_optional(struct R41169OptionalPanel *panel);
void L034_set_input_enabled(struct CharacterPanel *panel, int enabled);
int L034_get_selected(struct CharacterPanel *panel);
int scene_drawing_state(const struct W10DrawingInput *drawing);
#ifdef __cplusplus
}
#endif

#endif
