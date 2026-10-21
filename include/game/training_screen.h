#ifndef GAME_TRAINING_SCREEN_H
#define GAME_TRAINING_SCREEN_H
struct TrainingScreenA;
struct SummaryTrainingScene;

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
struct PairCollection;
#ifdef __cplusplus
struct TrainingScreenA : MenuScene {
    virtual void build();
    virtual ~TrainingScreenA();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001ba1d4 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
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
    unsigned char reserved456[0x1e];
    W11ControlScreen *small_controls;
};
#endif
#ifdef __cplusplus
extern "C" {
#endif
void func_001b8908(struct TrainingScreenA *screen);
void func_001b8238(struct TrainingScreenA *screen);
void func_001b8424(struct TrainingScreenA *screen);
void func_001b9f18(struct TrainingScreenA *screen);
#ifdef __cplusplus
extern const SceneMemberCallback data_002184b0;
#endif
void func_0016d6dc(struct TrainingScreenA *screen, int enabled);
void func_0016d600(struct TrainingScreenA *screen, int mode, int enabled);
#ifdef __cplusplus
}
#endif
struct ScalarCollection;
struct L041HandwritingPanel;
struct L041PromptTable;
struct MenuOptionGroup;
struct R41169OptionalPanel;
#ifdef __cplusplus
struct SummaryTrainingScene : MenuScene {
    virtual void build();
    virtual ~SummaryTrainingScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001e3270 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
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
#endif
#ifdef __cplusplus
extern "C" {
extern const SceneMemberCallback data_00218718;
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001dc170(int value);
extern void func_001dc230(int value);
extern struct TransformEntry *func_00174454(int group, int index);
extern void func_001dbc84(void);
extern void func_001e3050(struct SummaryTrainingScene *screen);
void func_001dd6c8(struct SummaryTrainingScene *screen);
extern int func_00173620(void);
extern void func_00176c6c(struct MessageLayout *layout);
extern void func_00176ae8(struct SelectionList24 *screen, int first, int second);
extern void func_0017673c(struct AnimationState *state);
extern void func_001762ac(struct Screen *screen, int mode);
extern void func_0016e1c8(struct Screen *screen, int index, int enabled);
extern void func_0017a164(struct Screen *screen, int state);
extern void func_00175dcc(struct WidgetController *panel);
extern void func_001b4aa0(struct L041HandwritingPanel *panel);
extern void func_00179d48(struct MenuControlSet *owner, int panel_index, int element_index, int enabled);
extern void func_00177c58(struct SummaryTrainingScene *screen, int selection);
extern void func_0016f930(const struct MenuControlSet *owner, int index);
extern void func_00175a90(struct MenuControlSet *owner, int index, int active);
extern void func_00175a5c(struct MenuControlSet *owner, int index,
                          float x, float y, float width, float height);
extern void func_001758e4(struct MenuOptionGroup *accessory);
extern int func_0017abf8(int channel, int sound);
int func_00172994(int channel);
#ifdef __cplusplus
}
#endif

struct L041PromptTable;
struct R41169OptionalPanel;
#ifdef __cplusplus
extern "C" {
#endif
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
