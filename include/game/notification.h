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

#ifdef __cplusplus
extern "C" {
#endif
extern struct NotificationState bss_00290cac;
extern void func_0015d6e0(int value);
#ifdef __cplusplus
}
#endif

#endif
