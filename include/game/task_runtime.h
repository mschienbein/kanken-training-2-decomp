#ifndef GAME_TASK_RUNTIME_H
#define GAME_TASK_RUNTIME_H
#include "game/synchronization.h"

/* [INFERENCE] One address-rooted runtime; activity slots and submission words
 * are independently used by the all-slots query and task submission routine. */
struct RuntimeActiveSlot { int identifier; unsigned reserved04; };
struct W24eSoundLoadContext;
struct RegionHeapOwner;
struct TaskRuntime {
    unsigned active;
    unsigned reserved04;
    int tag_count;
    int tags[48];
    unsigned char reservedcc[0x2f600 - 0xcc];
    struct RuntimeActiveSlot slots[16];
    unsigned char reserved2f680[0x28];
    struct W24eSoundLoadContext *sound;
    struct RegionHeapOwner *heap;
    unsigned char reserved2f6b0[0x31708 - 0x2f6b0];
    unsigned ready, pending;
    int exiting;
    unsigned char reserved31714[0x32720 - 0x31714];
    struct SignalState signal;
    unsigned values[48];
    int write_index;
    int read_index;
    int release_marker;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct TaskRuntime bss_0029f540;
extern int func_0015e2b8(int index);
#ifdef __cplusplus
}
#endif

#endif
