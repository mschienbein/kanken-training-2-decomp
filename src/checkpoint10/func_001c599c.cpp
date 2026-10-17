#include "game/stream_manager.h"

struct AdpcmCoefficients {
    short pairs[8][2];
};
struct AudioSlot {
    unsigned char reserved00[0x10];
    signed char mode;
    unsigned char state;
    unsigned char reserved12[2];
    AudioSlot *next;
    unsigned char reserved18[0x18];
};
/* INTEGRATOR: differs from include/game/audio_voice.h: recover a slot-list head
 * at 0x30, a dirty byte at 0x6f and the StreamMutex at 0x70 from reserved fields. */
struct AudioChannelState {
    unsigned char reserved00[0x1e];
    unsigned short flags1e;
    unsigned char reserved20[0x10];
    AudioSlot *head;
    unsigned char reserved34[0x6c - 0x34];
    unsigned short flags;
    unsigned char reserved6e;
    unsigned char dirty;
    StreamMutex mutex;
    AdpcmCoefficients coefficients;
};

class NotificationLock {
    StreamMutex *mutex;
public:
    explicit NotificationLock(StreamMutex *value) : mutex(value)
    {
        func_00192bd8(mutex);
    }
    ~NotificationLock()
    {
        func_00195d74(mutex);
    }
};

extern "C" void func_001c599c(AudioChannelState *receiver,
                              const AudioSlot *notification)
{
    NotificationLock lock(&receiver->mutex);
    for (AudioSlot *current = receiver->head; current;
         current = current->next) {
        if (current == notification) {
            if (notification->state != 3)
                receiver->dirty |= 2;
            break;
        }
    }
}
