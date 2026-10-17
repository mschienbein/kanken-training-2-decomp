#ifndef GAME_SOUND_NOTIFICATION_H
#define GAME_SOUND_NOTIFICATION_H

struct SoundNotification;
struct SoundNotificationReceiver;
struct SoundNotificationForwarder {
    unsigned char reserved00[0x68];
    struct SoundNotificationReceiver *receiver;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001c599c(struct SoundNotificationReceiver *receiver,
                          const struct SoundNotification *notification);
#ifdef __cplusplus
}
#endif

#endif
