#ifndef GAME_RESOURCE_RUNTIME_H
#define GAME_RESOURCE_RUNTIME_H
#include "game/runtime.h"

/* [INFERENCE] Resource-event names; array stride and active/owned-pointer fields
 * are established by cleanup and registry consumers, not zero stores alone. */
struct EventResource;
struct ResourceEvent {
    unsigned char reserved00[0x92];
    unsigned char kind[2];
    struct EventResource *resources[2];
    unsigned reserved9c;
    unsigned active;
    float reserveda4;
    unsigned paira8[2];
    unsigned char reservedb0[0x10];
    unsigned char cleanupc0;
    unsigned char reservedc1[0xb];
};
struct ResourceNode;
struct ResourceEventPool {
    unsigned reserved00[3];
    struct ResourceEvent *events;
    struct ResourceNode *first;
    struct ResourceNode *second;
    struct ResourceNode *third;
    int count;
    int active_count;
};
struct ResourceControlState {
    unsigned ready;
    unsigned state04;
    unsigned state08;
    unsigned char state0c;
    unsigned char reserved0d[3];
    int value;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct ResourceEventPool data_00242238;
extern struct ResourceControlState bss_00495c34, bss_00495c48, bss_00495c5c;
extern void func_0019d3b4(struct ResourceEvent *event);
extern void func_00183208(struct ResourceEvent *event);
#ifdef __cplusplus
}
#endif

#endif
