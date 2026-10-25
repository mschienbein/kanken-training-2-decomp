#include <preinclude.h>
#include "game/sound_wave.h"
#include <string.h>

using nw::snd::internal::WaveFile;

/* [INFERENCE] Both callers pass a null origin; the public ReadWaveInfo contract
 * identifies its pointer role, although this image's body does not read it. */
bool func_001f1cf0(const SoundWaveReader *reader, SoundWavePlaybackInfo *out,
                   const void *wave_data_origin)
{
    if (reader->relocated)
        return 0;
    const unsigned &version_threshold = 0x02010100;
    unsigned format = reader->info->encoding;
    if (format != 0 && format != 1)
        format = 3;
    out->format = format;
    out->channel_count = reader->info->GetChannelCount();
    out->sample_rate = reader->info->sampleRate;
    out->loop = reader->info->isLoop == 1;
    out->loop_start = reader->info->loopStartFrame;
    out->loop_end = reader->info->loopEndFrame;
    if (reader->file->version >= version_threshold)
        out->original_loop_start = reader->info->reserved;
    else
        out->original_loop_start = reader->info->loopStartFrame;
    for (int i = 0; i < reader->info->GetChannelCount(); ++i) {
        if (i < 2) {
            SoundWaveChannel &destination = out->channels[i];
            const WaveFile::ChannelInfo &channel = reader->info->GetChannelInfo(i);
            if (channel.referToAdpcmInfo.offset != 0) {
                const WaveFile::DspAdpcmInfo &adpcm = channel.GetDspAdpcmInfo();
                destination.decoder = adpcm.adpcmParam;
                destination.loop_state = adpcm.adpcmLoopParam;
            }
            destination.samples = channel.GetSamplesAddress(reader->data);
        }
    }
    return 1;
}
