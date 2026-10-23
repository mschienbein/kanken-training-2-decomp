#include "game/service_runtime.h"

struct ChannelPlayback {
    unsigned int key;
    short count;
    unsigned char reserved06[6];
    unsigned char started;
    unsigned char paused;
    unsigned char adjustment_mode;
    unsigned char reserved0f[0x1d];
    int samples;
    void *buffer;
    unsigned char reserved34[0x38];
    unsigned short dirty;
};

#include "game/audio_dsp.h"

void func_00154f3c(struct ChannelPlayback *playback)
{
    if (playback->dirty & 0x8000) {
        func_00155ab4(&bss_004b5284, playback->key & 0xff, playback->count);
        playback->dirty &= 0x7fff;
    }
}
