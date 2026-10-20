#ifndef GAME_STREAM_WORKER_H
#define GAME_STREAM_WORKER_H

#include "game/synchronization.h"

/* [INFERENCE] Only these actual job fields and seek/transfer slots are used.
 * Uncalled slots remain padding, not invented callable signatures. */
struct W22Stream;
struct W22StreamVTable {
    const void *reserved00[3];
    void (*seek)(struct W22Stream *stream, long long position, int mode);
    const void *reserved10[7];
    void (*transfer)(struct W22Stream *stream, int destination, int size);
};
struct W22Stream { const struct W22StreamVTable *vtable; };
struct W22StreamJob {
    unsigned char reserved00[12];
    unsigned cancelled;
    struct W22Stream *stream;
    unsigned char reserved14[0x44];
    int destination, length, source;
};
struct W22StreamWorker {
    unsigned char reserved00[12];
    unsigned pending, stopping;
    unsigned char reserved14[0x1014];
    struct W22StreamJob *job;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct W22StreamWorker bss_004962a8;
extern void func_00107ce4(struct SignalState *wake);
#ifdef __cplusplus
}
#endif

#endif
