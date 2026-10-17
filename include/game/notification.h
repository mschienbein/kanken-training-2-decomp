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

struct NotificationPayload {
    unsigned reserved00[12];
    unsigned char reserved30[2];
    unsigned char flags;
    unsigned char reserved33[0xb70d];
};

struct SavedNotification {
    int index;
    unsigned reserved;
    struct NotificationPayload payload;
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
