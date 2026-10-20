#ifndef GAME_PROGRESS_COUNTERS_H
#define GAME_PROGRESS_COUNTERS_H

struct TimedCounter {
    unsigned reserved00[2];
    int limit, current;
};
/* The base updater and the channel updater receive the same state object;
 * the three base counters start at +0x68, +0x88, and +0xb8. */
struct ExtendedTimedState {
    unsigned char reserved00[0x68];
    struct TimedCounter first;
    unsigned char reserved78[0x10];
    struct TimedCounter second;
    unsigned char reserved98[0x20];
    struct TimedCounter third;
    unsigned char reservedc8[0x3048 - 0xc8];
    struct TimedCounter channels[4];
    unsigned char reserved3088[0x328c - 0x3088];
    unsigned short enabled;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001d2be8(struct ExtendedTimedState *state);
#ifdef __cplusplus
}
#endif
#endif
