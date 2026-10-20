#include "game/audio_voice.h"
#include "game/stream_manager.h"


extern "C" void func_001c599c(AudioChannelState *receiver,
                              const AudioSlot *notification)
{
    StreamLockGuard lock(&receiver->mutex);
    for (AudioSlot *current = receiver->pending; current;
         current = current->next) {
        if (current == notification) {
            if (notification->state != 3)
                receiver->dirty |= 2;
            break;
        }
    }
}
