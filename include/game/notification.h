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

struct NotificationDayEntry {
    unsigned char flags;
    unsigned char reserved01[3];
    int day;
};
struct NotificationHistoryEntry {
    signed char status;
    unsigned char kind, category, reserved03;
    short x, y;
};
struct NotificationHistoryTrack {
    int current;
    struct NotificationHistoryEntry entries[10];
};

struct NotificationFields {
    unsigned char reserved00[0x32];
    unsigned char flags;
    unsigned char reserved33;
    signed char selection;
    signed char mode;
    unsigned char reserved36[2];
    int history_enabled;
    unsigned char reserved3c[0x0c];
    unsigned char saved_flag;
    unsigned char reserved49[0x258f];
    struct W10HistoryGrade history[12];
    unsigned char reserved61d8[0xc0];
    struct NotificationDayEntry days[180];
    unsigned char result_flags[12][0x640];
    struct NotificationHistoryTrack tracks[12];
    struct W10CourseProgress courses[2];
    signed char channels[12];
    unsigned char reservedb73c[4];
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
extern void func_001dbb70(int track, unsigned char category, unsigned short x,
                         unsigned short y, unsigned char kind);
/* [INFERENCE] Calendar key is a full word, compared with the serialized day. */
extern int func_00183070(int year, int month, int day);
extern unsigned char func_001dc100(int year, int month, int day);
extern void func_001dc180(void);
extern void func_001dc1ac(void);
extern void func_001dc1d8(void);
extern void func_001dc8e0(void);
extern struct TransformEntry *func_00174454(int group, int index);
extern void func_001dc918(int grade, int index, int value);
extern void func_001dc744(int grade, int index, int value);
extern void func_001dc170(int value);
extern void func_001dc230(int value);
extern void func_001dbcec(int grade, int category, int value);
extern void func_001dbfdc(int grade, int category, int value);
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


struct NotificationCache {
    unsigned char reserved00[0x748];
    unsigned generation;
    unsigned char reserved74c[0xdc];
    unsigned char active;
    unsigned char reserved829[3];
    unsigned count;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct NotificationCache bss_0028baf0;
/* [INFERENCE] Shared word result used as an index and cached unchanged. */
extern int func_001699b8(void);
#ifdef __cplusplus
}
#endif

#endif
