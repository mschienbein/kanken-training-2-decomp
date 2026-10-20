#ifndef GAME_DRAWING_SYSTEM_H
#define GAME_DRAWING_SYSTEM_H

struct RenderTarget;
struct RenderTargetVTable {
    void (*reserved[4])(void);
    const unsigned *(*handle)(struct RenderTarget *target);
};
struct RenderTarget { const struct RenderTargetVTable *vtable; };
/* [INFERENCE] Buffer tokens are full words; their ultimate semantic role is
 * distinct from the separately obtained transfer-surface descriptor. */
struct W23JDrawingBuffers {
    unsigned char reserved00[0x0c];
    int count;
    unsigned handles[2];
    int index;
};
/* [INFERENCE] Names are descriptive; initializer, render entry and mode callback
 * establish this single bss_002d3dd0 object and the used widths/offsets. */
struct DrawingSystem {
    unsigned status;
    unsigned char initialized;
    unsigned char mode_changed;
    unsigned char reserved06[2];
    unsigned visibility_mask;
    unsigned mode;
    struct W23JDrawingBuffers *ready;
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
extern void func_0019f0c4(int requested);
extern void func_0011de68(unsigned token, unsigned mask, unsigned offset,
                        unsigned count, unsigned flags);
#ifdef __cplusplus
}
#endif

#endif
