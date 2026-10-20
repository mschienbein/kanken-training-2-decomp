#ifndef GAME_SCHEDULER_H
#define GAME_SCHEDULER_H

struct W19aScheduledTask;
struct W19aScheduler;
struct W19aTaskSelection;
#include "game/service_state.h"
/* One call-compatible C callback type, including the real int-result providers.
 * Procedure callbacks have unspecified results that MUST be discarded. Their C
 * definitions may fall through (C11 6.9.1p12); they must not become C++ bodies.
 * Real result providers keep their returns. No machine-scratch result is inferred. */
#ifdef __cplusplus
extern "C" {
#endif
typedef int (*ScheduledTaskCallback)(void);
struct ScheduledTaskBinding { ScheduledTaskCallback callback; int wait; };
struct ScheduledTask {
    short enabled;
    short previous, next;
    unsigned short priority;
    struct ScheduledTaskBinding binding;
    int repeat;
    int reserved14;
};
struct Scheduler {
    struct ScheduledTask tasks[64];
    int current;
    int next;
    short end, begin;
    int pending;
};

/* Distinct words at +0x14 and +0x18 are used by transition activation and reset. */
struct TransitionState {
    unsigned char reserved00[0x14];
    unsigned enabled;
    int active;
};
extern struct Scheduler bss_00495c8c;
extern struct TransitionState bss_00290c90;
extern int func_0019b54c(int task);
extern int func_0019b564(void);
extern void func_00195ad0(unsigned int index);
/* C mutation procedure: its unspecified result MUST be discarded. */
extern int func_00190688(int index, ScheduledTaskCallback callback);
extern void func_001071cc(void);
extern void func_001074d0(void);
extern void func_00107dbc(void);
extern void func_001071ec(void);
extern void func_00110aa4(void);
extern void func_00195168(void);
extern void func_0012d1cc(void);
extern void func_0012d1d8(int count, int preserve);
extern void func_0012ced8(void);
extern void func_001951dc(unsigned int handle);
extern void func_0012d1c8(void);
extern int func_0013db40(void);
extern int func_001415d0(void);
extern int func_001415e8(void);
extern int func_00141c70(void);
extern int func_001900a8(void);
extern int func_00110a70(void);
#ifdef __cplusplus
}
#endif


#endif
