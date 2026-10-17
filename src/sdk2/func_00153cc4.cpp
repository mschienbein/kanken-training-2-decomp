#include "game/prefetch_stream.h"
extern "C" void func_00153cc4(StreamPlayer *player, const StreamPlaybackInfo *info)
{
    unsigned index;
    int channel;
    for (index = 0; index < player->track_count; ++index) {
        const PrefetchTrackInfo *input = &info->tracks[index];
        player->tracks[index].volume = input->volume;
        player->tracks[index].pan = input->pan;
        player->tracks[index].surround_pan = input->surround_pan;
        player->tracks[index].send_a = input->send_a;
        player->tracks[index].send_b = input->send_b;
        player->tracks[index].send_c = input->send_c;
        player->tracks[index].extra7 = input->extra7;
        player->tracks[index].extra8 = input->extra8;
        player->tracks[index].extra9 = input->extra9;
        player->tracks[index].lowpass = input->lowpass;
        player->tracks[index].channel_count = input->channel_count;
        /* ORIGINAL BUG: encoded counts and channel indices are not bounds-checked. */
        for (channel = 0; channel < input->channel_count; ++channel)
            player->tracks[index].channels[channel] = &player->channels[input->channel_indices[channel]];
    }
}
