#ifndef GAME_DRAWING_SYSTEM_H
#define GAME_DRAWING_SYSTEM_H

struct RenderTarget;
struct RenderTargetVTable {
    void (*reserved[4])(void);
    const unsigned *(*handle)(struct RenderTarget *target);
};
struct RenderTarget { const struct RenderTargetVTable *vtable; };
/* [INFERENCE] Names are descriptive; initializer, render entry and mode callback
 * establish this single bss_002d3dd0 object and the used widths/offsets. */
struct DrawingSystem {
    unsigned status;
    unsigned char initialized;
    unsigned char mode_changed;
    unsigned char reserved06[2];
    unsigned visibility_mask;
    unsigned mode;
    unsigned ready;
    unsigned flags;
    unsigned char reserved18[0x8c];
    struct RenderTarget *target;
    unsigned active;
    unsigned char reservedac[0x19c];
    unsigned current;
    unsigned requested;
    unsigned phase;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct DrawingSystem bss_002d3dd0;
extern void func_0011d330(unsigned flags);
extern void func_0011dcec(int mode);
extern void func_0019b74c(int display);
#ifdef __cplusplus
}
#endif

#endif
