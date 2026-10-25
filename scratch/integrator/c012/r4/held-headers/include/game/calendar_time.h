#ifndef GAME_CALENDAR_TIME_H
#define GAME_CALENDAR_TIME_H
#include "game/memory.h"
#ifdef __cplusplus
/* Names-admission ties the calendar constructor to the original call.
 * [INFERENCE] Descriptive timestamp/buffer identities and opaque byte roles. */
struct CalendarTime {
    long long value;
    CalendarTime() : value(0) {}
    CalendarTime(int year, int month, int day, int hour,
                 int minute, int second, int millisecond);
};
struct TimestampBuffers {
    unsigned char first[0x80];
    unsigned char second[0x100];
    unsigned char third[0x180];
    unsigned char reserved300[0x20];
    CalendarTime timestamp;
    unsigned char reserved328[5];
    unsigned char active;
    TimestampBuffers();
};
#endif
#endif
