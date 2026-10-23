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

extern int func_00155a44(struct ServiceCoordinator *coordinator, int channel);

void func_00155120(struct ChannelPlayback *playback)
{
    func_00155a44(&bss_004b5284, playback->key & 0xff);
    playback->started = 1;
}
