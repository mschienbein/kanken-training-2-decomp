#ifndef GAME_NOTIFICATION_H
#define GAME_NOTIFICATION_H

/* Partial notification state: reset writes the leading halfword; the consumer
 * at func_001415ec reads the pending state and value as signed bytes. */
struct NotificationState {
    unsigned short status;
    signed char pending;
    signed char value;
    unsigned char enabled;
    unsigned char acknowledged;
};

struct NotificationFields {
    unsigned char reserved00[0x32];
    unsigned char flags;
    unsigned char reserved33[0xb70d];
};

/* UTF-16 and word-copy clients address one serialized storage object. */
union NotificationStorage {
    unsigned words[0x2dd0];
    unsigned short text[0x5ba0];
    struct NotificationFields fields;
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
extern void func_0015d6e0(int value);
#ifdef __cplusplus
}
#endif

#endif
