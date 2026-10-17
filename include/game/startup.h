#ifndef GAME_STARTUP_H
#define GAME_STARTUP_H

struct ApplicationControl;
struct ResourceOptions;
struct StartupCommandPool;
struct StartupResources;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ApplicationControl {
    unsigned char mode;
    unsigned char option;
    unsigned char reserved02[2];
    int suspended;
    void (*callback)(void);
};

struct ResourceEvent;
struct ResourceOptions {
    unsigned active;
    unsigned pending;
    unsigned reserved08[2];
    void *allocation;
    struct ResourceEvent *event;
    int enabled;
    int group;
    int count;
    int option_a;
    int option_b;
};

struct StartupResources {
    int initialized;
    int status;
    unsigned counter;
    unsigned pending;
    int ready;
    struct StartupCommandPool *commands;
};

struct RestorableState { unsigned current, saved; };

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned char func_001081ec(void);
extern void func_0019f324(unsigned char mode, unsigned char option);
extern void func_001070e8(void);
extern void func_00107140(void);
extern void func_00107a88(void (*callback)(void));
extern struct ApplicationControl bss_0049629c;
extern struct RestorableState data_0023fdc8;
extern void func_001906b8(int suspended);
extern void func_001906a8(void (*callback)(void));
extern void func_00190688(int index, void (*callback)(void));
extern void func_0011ef60(void);
extern void func_0011f234(void);
extern unsigned char func_00190678(void);
extern void func_00195a50(void);
extern void func_0011f3a8(void);
extern struct ResourceOptions bss_0049db78;
extern struct StartupResources bss_0028d944;
extern void func_00141944(void);
extern int func_00145adc(void);
extern void func_0010433c(void);
extern int func_00107130(void);
extern void func_001109fc(void);
extern void func_00110a28(void);
extern void func_0013604c(int group);
extern void func_0013605c(int count);
extern void func_0013606c(int option);
extern void func_0013607c(int option);
extern void func_001360dc(int enabled);
extern void func_0013de78(int initialized);
extern void func_0013deb4(void);
extern void func_0014562c(void);
#ifdef __cplusplus
}
#endif

#endif
