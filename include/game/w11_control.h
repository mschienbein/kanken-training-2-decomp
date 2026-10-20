#ifndef GAME_W11_CONTROL_H
#define GAME_W11_CONTROL_H
#include "game/animation.h"
#include "game/menu_collections.h"
struct W11ControlScreen {
    struct MaskHeader mask;
    unsigned selection_word;
    struct SmallLabelHandle *slots[10];
    int layout, active, selection;
};
/* 001eac48 allocates 72 groups of 0x9c bytes. The producer at 001ea020
 * and scene +0x48 stores bind this owner to both label API families. */
struct VisualLabelFormat {
    int kind;
    unsigned reserved04, flags;
    unsigned reserved0c[4];
};
struct VisualLabelGroup10 {
    unsigned reserved00;
    struct DisplayObject *objects[10];
    short count;
    signed char states[10];
    unsigned char reserved38[0x40];
    struct WidgetPosition anchor, position, offset, scale;
    unsigned char color[4];
};
struct VisualLabelSet72 {
    struct MaskHeader mask;
    unsigned selection_word;
    struct VisualLabelFormat formats[72];
    struct VisualLabelGroup10 *groups[72];
};
struct W11jGroupSlot { struct DisplayObject *object; int state; };
struct W11jWidgetGroup {
    unsigned char reserved00[0x0c];
    struct W11jGroupSlot *slots[10];
    signed char active;
    unsigned char enabled, complete, reserved37;
    float elapsed, duration;
    int mode;
};
struct AnimationState {
    unsigned char reserved00[0x10];
    void *primary;
    unsigned char reserved14[0x30];
    float start, finish;
    unsigned char reserved4c[8];
    int enabled;
    float lower, upper, current;
};
struct TrainingScreenA;
struct TrainingScreenB;
/* W11AnimatedScreenAVTable is represented by the real training scene class. */
/* W11AnimatedScreenBVTable is represented by the real training scene class. */
/* TrainingScreenA is represented by the real training scene class. */
/* TrainingScreenB is represented by the real training scene class. */
#ifdef __cplusplus
extern "C" {
#endif
extern int func_0017a210(const struct AnimationState *state);
extern struct AnimationState *func_00167284(void);
extern struct AnimationState bss_0024424c;
extern volatile unsigned data_0023ecc0;
extern struct AnimationState *func_001b43bc(struct AnimationState *state);
extern struct AnimationState *func_001b44e8(struct AnimationState *state);
extern void func_001b0600(struct W11ControlScreen *screen);
extern void func_0016d554(struct W11ControlScreen *screen, int state);
extern void func_001741f8(struct VisualLabelSet72 *owner, int index, float x, float y);
extern void func_0016f9a8(struct VisualLabelSet72 *owner, int index, float x, float y);
extern void func_00175d90(struct VisualLabelSet72 *owner);
extern void func_0016ff68(struct W11ControlScreen *screen, int index, int enabled);
extern void func_0016ff08(struct W11ControlScreen *screen, int index, float x, float y);
#ifdef __cplusplus
}
#endif
#endif
