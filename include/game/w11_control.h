#ifndef GAME_W11_CONTROL_H
#define GAME_W11_CONTROL_H
#include "game/animation.h"
struct W11ControlScreen {
    unsigned char reserved00[0x0c];
    struct WidgetSlot *slots[10];
    int layout, active, selection;
};
struct W11jVisualNode {
    unsigned char reserved00[0x90];
    float scale_x, scale_y;
    unsigned char color[4];
};
struct W11jVisualOwner {
    unsigned char reserved00[0x7ec];
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
    unsigned char reserved14[0x30];
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
extern void func_001b0600(struct W11ControlScreen *screen);
extern void func_0016d554(struct W11ControlScreen *screen, int state);
extern void func_0016ff68(struct W11ControlScreen *screen, int index, int enabled);
extern void func_0016ff08(struct W11ControlScreen *screen, int index, float x, float y);
#ifdef __cplusplus
}
#endif
#endif
