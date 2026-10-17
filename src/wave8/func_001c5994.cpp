#include "game/sound_notification.h"

extern "C" void func_001c5994(SoundNotificationForwarder *object,
                              const SoundNotification *notification)
{
    func_001c599c(object->receiver, notification);
}
