#ifndef GAME_NOTIFICATION_H
#define GAME_NOTIFICATION_H

struct W14ValuePair;

#include "game/notification_selection.h"

/* Partial notification state: reset writes the leading halfword; the consumer
 * at func_001415ec reads the pending state and value as signed bytes. */
struct NotificationState {
    unsigned short status;
    signed char pending;
    signed char value;
    unsigned char enabled;
    unsigned char acknowledged;
};

struct W10HistoryGrade {
    short lists[32][20];
};

struct W10CourseProgress {
    signed char grade;
    signed char stage;
    short progress;
};

struct NotificationFields {
    unsigned char reserved00[0x32];
    unsigned char flags;
    unsigned char reserved33;
    signed char selection;
    signed char mode;
    unsigned char reserved36[0x25a2];
    struct W10HistoryGrade history[12];
    unsigned char reserved61d8[0x660];
    unsigned char result_flags[12][0x640];
    unsigned char reservedb338[0x3f0];
    struct W10CourseProgress primary;
    struct W10CourseProgress secondary;
    unsigned char reservedb730[0x10];
};

/* UTF-16 and word-copy clients address one serialized storage object. */
struct NotificationSelection {
    unsigned char reserved00[0x58];
    struct W14ValuePair primary[12][150];
    struct W14ValuePair secondary[12][50];
    struct W10HistoryGrade history[12];
};

union NotificationStorage {
    unsigned words[0x2dd0];
    unsigned short text[0x5ba0];
    struct NotificationFields fields;
    struct NotificationSelection selection;
};

struct NotificationPayload {
    union NotificationStorage storage;
};

struct TransformEntry {
    unsigned int reserved00;
    float x;
    float y;
    unsigned short count;
    unsigned short flags;
};

struct SavedNotification {
    int index;
    unsigned reserved;
    struct NotificationPayload payload;
    unsigned char reservedb748[0xe8];
#ifndef __cplusplus
    struct TransformEntry transforms[][4];
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct NotificationState bss_00290cac;
extern struct NotificationPayload bss_002476b0[];
extern struct SavedNotification bss_00280af0;
/* Interior symbol for the very same payload at bss_00280af0 + 8, not a second
 * storage allocation. History code uses the payload address as its literal root. */
extern struct NotificationPayload bss_00280af8;
extern void func_0015d6e0(int value);
extern void func_00190e44(struct NotificationPayload *payload);
extern unsigned short *func_00172008(int index);
extern void func_00172d48(int index);
extern void func_0016fccc(int selection);
extern int func_001758d4(void);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
extern const short data_002181b4[5];
extern void func_001dbbd4(int mode);
#ifdef __cplusplus
}
#endif

#endif
