#ifndef GAME_DATE_TIME_H
#define GAME_DATE_TIME_H

struct DateTime;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct DateTime {
    long long milliseconds;
#ifdef __cplusplus
    DateTime() : milliseconds(0) {}
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct DateTime func_001910d4(void);
extern int func_001efbf8(const struct DateTime *time);
extern int func_001efc64(const struct DateTime *time);
extern int func_001efaac(const struct DateTime *time);
extern void func_0015dc70(int *year, int *month, int *day, int *weekday,
                   int *hour, int *minute, int *second, int *millisecond);
#ifdef __cplusplus
}
#endif

#endif
