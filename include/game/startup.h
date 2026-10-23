#ifndef GAME_STARTUP_H
#define GAME_STARTUP_H

struct ApplicationControl;
struct ResourceOptions;
struct StartupCommandPool;
struct StartupResources;

#include "game/runtime.h"
#include "game/scheduler.h"

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

/* Signed: func_00135908 converts `current` with a signed integer-to-float conversion. */
struct RestorableState { int current, saved; };

struct StartupItem { int value, flags, id; };
/* Callback storage is established by 00195b6c and exact 0049c87c/0049c880
 * indirect consumers in 00105444, not inferred from zero initialization. */
struct StartupState {
    int a;
    void (*b)(void);
    void (*c)(void);
    unsigned char enabled, active, ready;
    char reserved;
    int position, step;
    struct StartupItem items[8];
    int count;
};

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned char func_001081ec(void);
extern void func_00135c10(int count);
extern void func_0019f324(unsigned char mode, unsigned char option);
extern int func_001070e8(void);
extern int func_00107140(void);
/* C mutation procedure: its unspecified result MUST be discarded. */
extern int func_00107a88(ScheduledTaskCallback callback);
extern struct ApplicationControl bss_0049629c;
extern struct RestorableState data_0023fdc8;
/* C mutation procedure: its unspecified result MUST be discarded. */
extern int func_001906b8(int suspended);
extern void func_001906a8(void (*callback)(void));
/* C registration procedures: unspecified results MUST be discarded. */
extern int func_00190688(int index, ScheduledTaskCallback callback);
extern int func_0019f338(int index, ScheduledTaskCallback callback, unsigned priority);
extern void func_00195b6c(int mode, void (*initialize)(void), void (*callback)(void),
                          void (*complete)(void), void (*finish)(void));
extern struct StartupState bss_0049c878;
extern void func_0011ef60(void);
extern int func_0011f234(void);
extern unsigned char func_00190678(void);
extern void func_00195a50(void);
extern int func_0011f3a8(void);
extern struct ResourceOptions bss_0049db78;
extern struct StartupResources bss_0028d944;
extern void func_00141944(void);
extern int func_00145adc(void);
extern int func_0010433c(void);
extern int func_00107130(void);
extern int func_001109fc(void);
extern int func_00110a28(void);
extern void func_0013604c(int group);
extern void func_0013605c(int count);
extern void func_0013606c(int option);
extern void func_0013607c(int option);
extern void func_001360dc(int enabled);
extern void func_0013de78(int initialized);
extern void func_0013deb4(void);
extern int func_0014562c(void);
#ifdef __cplusplus
}
#endif

#endif
