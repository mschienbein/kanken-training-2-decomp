#ifndef GAME_TASK_RUNTIME_H
#define GAME_TASK_RUNTIME_H
#include "game/synchronization.h"

/* [INFERENCE] One address-rooted runtime; activity slots and submission words
 * are independently used by the all-slots query and task submission routine. */
struct RuntimeActiveSlot { int identifier; unsigned reserved04; };
struct TaskRuntime {
    unsigned active;
    unsigned char reserved04[0x2f5fc];
    struct RuntimeActiveSlot slots[16];
    unsigned char reserved2f680[0x2088];
    unsigned ready, pending;
    unsigned char reserved31710[0x1010];
    struct SignalState signal;
    unsigned values[48];
    int write_index;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct TaskRuntime bss_0029f540;
#ifdef __cplusplus
}
#endif

#endif
