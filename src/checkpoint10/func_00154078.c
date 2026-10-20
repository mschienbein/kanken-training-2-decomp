#include "game/audio_voice.h"
struct HwrUpdateChannel {
    unsigned char reserved00[0x68];
    struct AudioChannelState *state;
};
struct HwrUpdateOwner {
    unsigned char reserved00[0x18dc];
    struct HwrUpdateChannel *channels[24];
};

void func_00154078(struct HwrUpdateOwner *owner)
{
    int i;
    for (i = 0; i < 24; ++i)
        func_0014e218(owner->channels[i]->state);
}
