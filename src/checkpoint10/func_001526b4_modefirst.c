#include "game/audio_voice.h"
#include "game/stream_manager.h"

struct AudioBiquadCoefficients {
    short b0, b1, b2, a1, a2;
};
/* INTEGRATOR: differs from include/game/audio_voice.h: named fields occupy the
 * existing padding, including the pending list at 0x30 and mutex at 0x70. */
struct AudioChannelState {
    unsigned int id;
    unsigned short generation;
    unsigned char reserved06[6];
    unsigned char started;
    unsigned char mode;
    unsigned char reserved0e[6];
    struct AudioBiquadCoefficients filter;
    unsigned short flags1e;
    unsigned char reserved20[0x10];
    struct AudioSlot *pending;
    unsigned short pending_count;
    unsigned short pending_limit;
    unsigned char reserved38[0x34];
    unsigned short flags;
    unsigned char reserved6e[2];
    struct StreamMutex mutex;
    struct AdpcmCoefficients coefficients;
};
/* INTEGRATOR: differs from include/game/audio_voice.h: mode and biquad staging
 * fields occupy existing padding; the state pointer remains at 0x68. */
struct AudioChannel {
    unsigned char reserved00[4];
    unsigned char mode;
    unsigned char reserved05[7];
    struct AudioBiquadCoefficients filter;
    unsigned char reserved16[0x52];
    struct AudioChannelState *state;
};

extern void func_0014fc98(struct AudioChannelState *state);


void func_001526b4(struct AudioChannel *channel, int mode)
{
    int next_mode = mode;
    channel->mode = next_mode;
    if (next_mode == 1) {
        struct AudioChannelState *state = channel->state;
        func_0014fc98(state);
    }
    func_001526e0(channel->state, next_mode);
}
