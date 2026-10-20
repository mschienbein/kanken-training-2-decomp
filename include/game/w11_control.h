#ifndef GAME_W11_CONTROL_H
#define GAME_W11_CONTROL_H
#include "game/animation.h"
struct W11ControlScreen {
    unsigned char reserved00[0x0c];
    struct WidgetSlot *slots[10];
    int layout, active, selection;
};
/* [INFERENCE] Numeric format roles follow the actual renderer dispatcher.
 * This is the physical prefix before the shared node vector, not proof of a
 * complete native owner or validation of arbitrary indices. */
struct W11jNumberFormat {
    unsigned kind;
    unsigned reserved04[6];
};
struct W11jVisualNode {
    unsigned reserved00;
    struct DisplayObject *objects[10];
    short count;
    unsigned char reserved2e[0x52];
    float position[2];
    unsigned char reserved88[8];
    float scale_x, scale_y;
    unsigned char color[4];
};
struct W11jVisualOwner {
    unsigned char reserved00[0x0c];
    struct W11jNumberFormat formats[72];
#ifndef __cplusplus
    struct W11jVisualNode *nodes[];
#endif
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
    unsigned char reserved14[0x20];
    unsigned char disabled, has_secondary;
    unsigned char reserved36[0x0e];
    float start, finish;
    unsigned char reserved4c[8];
    int enabled;
    float lower, upper, current;
};
struct W11AnimatedScreenA;
struct W11AnimatedScreenB;
struct W11AnimatedScreenAVTable {
    unsigned char reserved00[0x18];
    void (*begin_frame)(struct W11AnimatedScreenA *);
    unsigned char reserved1c[4];
    void (*end_frame)(struct W11AnimatedScreenA *);
};
struct W11AnimatedScreenBVTable {
    unsigned char reserved00[0x18];
    void (*begin_frame)(struct W11AnimatedScreenB *);
    unsigned char reserved1c[4];
    void (*end_frame)(struct W11AnimatedScreenB *);
};
struct W11AnimatedScreenA {
    const struct W11AnimatedScreenAVTable *vtable;
    unsigned char reserved04[0x68];
    const struct AnimationState *animation;
    unsigned char reserved70[0x404];
    struct W11ControlScreen *controls;
};
struct W11AnimatedScreenB {
    const struct W11AnimatedScreenBVTable *vtable;
    unsigned char reserved04[0x68];
    const struct AnimationState *animation;
    unsigned char reserved70[0x460];
    struct W11ControlScreen *controls;
};
#ifdef __cplusplus
extern "C" {
#endif
extern int func_0017a210(const struct AnimationState *state);
extern struct AnimationState *func_00167284(void);
extern struct AnimationState bss_0024424c;
extern volatile unsigned data_0023ecc0;
extern struct AnimationState *func_001b43bc(struct AnimationState *state);
extern struct AnimationState *func_001b44e8(struct AnimationState *state);
/* [INFERENCE] The original initializer/teardown share the +0xc slot and
 * +0x38/+0x3c/+0x40 owned allocations inside this partial animation owner. */
extern void func_001b3c64(struct AnimationState *state);
extern void func_001b0600(struct W11ControlScreen *screen);
extern void func_0016d554(struct W11ControlScreen *screen, int state);
extern void func_001741f8(struct W11jVisualOwner *owner, int index, float x, float y);
extern void func_0016f9a8(struct W11jVisualOwner *owner, int index, float x, float y);
extern void func_00175d90(struct W11jVisualOwner *owner);
extern void func_00166f1c(struct W11jVisualOwner *owner, int index, int enabled);
extern void func_00166f20(struct W11jVisualOwner *owner, int index, int enabled);
extern void func_001ea314(struct W11jVisualOwner *owner, int index,
                          const struct W11jNumberFormat *format, int value);
extern void func_001ea1cc(struct W11jVisualOwner *owner, int index,
                          const struct W11jNumberFormat *format, int value);
extern void func_001ea0cc(struct W11jVisualOwner *owner, int index,
                          const struct W11jNumberFormat *format, int value);
extern void func_0016ff68(struct W11ControlScreen *screen, int index, int enabled);
extern void func_0016ff08(struct W11ControlScreen *screen, int index, float x, float y);
extern void func_0016de28(struct AnimationState *state, int enabled);
extern void func_0016f208(struct AnimationState *state, int index, int enabled);
#ifdef __cplusplus
}
#endif
#endif
