#ifndef GAME_MENU_CONTROLLER_H
#define GAME_MENU_CONTROLLER_H
struct MenuExtentScene;
struct SelectionMenuScene;
struct InitializationMenuScene;
struct MenuResetScene;
struct ExitSelectionScene;
struct Tu441SecondaryController;
struct W18DisplayCollection;

#include "game/animation.h"
#include "game/selection.h"
#include "game/transition_controller.h"
#include "game/menu_scene.h"

#ifdef __cplusplus
/* [INFERENCE] Native class names; concrete constructor/table identities are
 * respectively 001a29f4/00223544, 001a7420/00223570,
 * 001ab51c/0022359c, 001acb90/002235c8 and 001be104/00223720. */
struct Tu441SecondaryController;
struct W18DisplayCollection;
struct MenuExtentScene : MenuScene {
    virtual void build();
    virtual ~MenuExtentScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001a29f4 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int depth;
    int stages[3], previous_stages[3];
    float elapsed;
    int selection_value, secondary;
    unsigned char reservedb8[0x28];
    void *member;
    Tu441SecondaryController *secondary_member;
};
struct SelectionMenuScene : MenuScene {
    virtual void build();
    virtual ~SelectionMenuScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001a7420 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int depth;
    int stages[3], previous_stages[3];
    float elapsed;
    int selection_value, secondary;
};
struct InitializationMenuScene : MenuScene {
    virtual void build();
    virtual ~InitializationMenuScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001ab51c constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int depth;
    int stages[3], previous_stages[3];
    float elapsed;
    int selection_value, secondary;
};
struct MenuResetScene : MenuScene {
    virtual void build();
    virtual ~MenuResetScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001acb90 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int depth;
    int current[2], previous[2];
    float elapsed;
    int state, mode;
    unsigned reservedb0;
    W18DisplayCollection *objects;
};
struct ExitSelectionScene : MenuScene {
    virtual void build();
    virtual ~ExitSelectionScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001be104 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int depth;
    int current[2], previous[2];
    float elapsed;
    int last_selection;
    W18DisplayCollection *objects;
};
extern "C" {
#endif
/* [INFERENCE] Erased secondary update and state-accessor identities remain unknown. */
void tu441_update_secondary(struct Tu441SecondaryController *controller);
signed char tu441_transition_active(const struct PairCollection *controller);
int func_00171e30(void);
void func_001c1080(void *object);
void func_0016337c(struct MenuExtentScene *state, int current);
int func_00171d88(const struct MenuControlSet *owner, int index, int child_index);
void func_00171d40(struct MenuControlSet *owner, int index, int child_index, int text);
void func_00171db8(struct MenuControlSet *owner, int index, int child_index, float x, float y);
void func_001ea6d8(struct VisualLabelSet72 *layout, int index, float x, float y);
void func_001a3ee0(struct SelectionMenuScene *screen);
void func_001abf24(struct MenuResetScene *screen);
void func_001ab680(struct MenuResetScene *screen);
void func_001bda14(struct ExitSelectionScene *screen);
void func_001bdb70(struct ExitSelectionScene *screen);
void func_00171000(struct W18DisplayCollection *handles, int index, float x, float y);
void func_0016c55c(struct ExitSelectionScene *scene);
#ifdef __cplusplus
}
#endif

#endif
