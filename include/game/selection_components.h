#ifndef GAME_SELECTION_COMPONENTS_H
#define GAME_SELECTION_COMPONENTS_H
#include "game/screen.h"
#include "game/input.h"
#include "game/resource_tables.h"

/* Published component at DisplayUpdateManager +0x88. The tail wrapper also
 * supplies its reached count even though the input updater ignores that word. */
struct W19SelectionPanel {
    struct WidgetController controller;
    struct WidgetSlot *slots[25];
    struct InputSnapshot *inputs[12];
    int selected, hovered, previous_hovered, value;
    unsigned reservedb0[2];
    int enabled;
};
struct W23FlowScreen {
    unsigned char reserved00[0x90];
    int stack_index;
    int modes[2];
    int previous_modes[2];
    unsigned char reserveda4[8];
    int mode;
};
struct DeferredResourceEntry16 {
    unsigned key;
    unsigned fields[3];
};
struct DeferredResourceList16 {
    unsigned char reserved00[0x0c];
    int state;
    struct DeferredResourceEntry16 *entries;
    int count;
};
struct DeferredResourceList20 {
    unsigned char reserved00[8];
    int state;
    struct DetailEntry *entries;
    int count;
};
struct Component001b3564;
struct Component001e9e7c;
struct TransformTarget;
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001adc0c(struct W19SelectionPanel *panel, int index);
extern void func_0016bde0(struct TransformTarget *const *objects, int index,
    int mode, float x, float y);
extern void func_001c3d14(struct TransformTarget *const *objects, int index,
    int selection, int detail);
extern void func_001adff8(struct W19SelectionPanel *panel);
extern void func_001ae00c(struct W19SelectionPanel *panel, int count);
extern void func_001abf24(struct W23FlowScreen *screen);
extern void func_001acb38(struct W23FlowScreen *screen);
extern void func_001ab680(struct W23FlowScreen *screen);
extern void func_001737c0(struct W23FlowScreen *screen, int enabled);
extern void func_001dcdd0(struct DeferredResourceList16 *list);
extern void func_001dcf4c(struct DeferredResourceList20 *list);
extern void func_001b3564(struct Component001b3564 *controller);
extern void func_001b38c4(struct Component001b3564 *controller, int index);
extern void func_001e9e7c(struct Component001e9e7c *collection);
extern void func_001e8c74(struct Component001e9e7c *collection, int index);
#ifdef __cplusplus
}
#endif
#endif
