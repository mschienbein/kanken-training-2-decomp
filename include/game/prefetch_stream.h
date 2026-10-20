#ifndef GAME_PREFETCH_STREAM_H
#define GAME_PREFETCH_STREAM_H
#include "game/audio_voice.h"

struct PrefetchChannelEntry { unsigned start, size, reserved08[2]; int data_offset; };
struct PrefetchDataBlock {
    unsigned count;
#ifndef __cplusplus
    struct PrefetchChannelEntry entries[];
#endif
};

struct AdpcmChannelParameters {
    short coefficients[16];
    short predictor_scale;
    short history1;
    short history2;
};
#ifdef __cplusplus
struct StreamChannel {
#ifdef __cplusplus
    StreamChannel();
#endif
    void *allocation;
    nw::snd::internal::driver::Voice *channel;
    struct RecordState blocks[32];
    struct AdpcmLoopState loop_states[32];
};
#endif
struct PrefetchTrackInfo {
    unsigned char volume;
    unsigned char pan;
    unsigned char surround_pan;
    unsigned char lowpass;
    unsigned char send_a;
    unsigned char send_b;
    unsigned char send_c;
    unsigned char extra7;
    unsigned char extra8;
    unsigned char extra9;
    unsigned char channel_count;
    unsigned char channel_indices[2];
};
struct StreamTrack {
    unsigned reserved00;
    struct StreamChannel *channels[2];
    unsigned char channel_count;
    unsigned char volume;
    unsigned char pan;
    unsigned char surround_pan;
    unsigned char send_a;
    unsigned char send_b;
    unsigned char send_c;
    unsigned char extra7;
    unsigned char extra8;
    unsigned char extra9;
    unsigned char lowpass;
    unsigned char reserved17[13];
};
struct StreamPlaybackInfo {
    unsigned char encoding;
    unsigned char reserved01[3];
    unsigned sample_rate;
    unsigned char loop;
    unsigned char reserved09[3];
    unsigned loop_start;
    unsigned sample_count;
    unsigned seek_interval;
    unsigned seek_entry_size;
    unsigned samples_per_block;
    unsigned block_size;
    unsigned final_block_padded_size;
    unsigned final_block_samples;
    unsigned channel_count;
    unsigned track_count;
    struct PrefetchTrackInfo tracks[4];
};
struct PrefetchStreamInfo {
    unsigned char encoding;
    unsigned char loop;
    unsigned char channel_count;
    unsigned char reserved03;
    unsigned sample_rate;
    unsigned loop_start;
    unsigned sample_count;
    unsigned block_count;
    unsigned block_size;
    unsigned samples_per_block;
    unsigned final_block_size;
    unsigned final_block_samples;
    unsigned final_block_padded_size;
    unsigned reserved28[7];
    unsigned seek_interval;
    unsigned seek_entry_size;
};
struct PrefetchFileReader {
    const struct PrefetchFileHeader *file;
    const struct PrefetchInfoBlock *info;
    const struct PrefetchDataBlock *data;
};
struct PrefetchChannelInfo {
    unsigned start;
    unsigned size;
    const void *data;
};
struct PrefetchReferenceTable {
    unsigned count;
};
struct StreamBufferState {
    unsigned request_id;
    unsigned context;
};
#ifdef __cplusplus
struct StreamPlayer {
    const void *vtable;
    unsigned char reserved04[8];
    unsigned char loading;
    unsigned char reserved0d[2];
    unsigned char stopped;
    unsigned char reserved10[64];
    unsigned char prepared;
    unsigned char ready;
    unsigned char reserved52;
    unsigned char channels_allocated;
    unsigned char reserved54[2];
    unsigned char complete_block;
    unsigned char reserved57;
    unsigned reserved58;
    unsigned pending_buffer_count;
    unsigned reserved60;
    struct StreamHeap *heap;
    unsigned block_count;
    unsigned next_buffer;
    unsigned reserved70[3];
    unsigned char start_offset_type;
    unsigned char reserved7d[3];
    int start_offset;
    unsigned reserved84;
    struct StreamPlaybackInfo format;
    unsigned char reservedf0[28];
    const struct PrefetchFileHeader *source;
    struct AdpcmCoefficients coefficients[8];
    struct PrefetchChannelInfo prefetch;
    unsigned current_sample;
    unsigned channel_count;
    unsigned track_count;
    struct StreamChannel channels[8];
    struct StreamTrack tracks[4];
    struct StreamBufferState buffer_states[32];
};
#endif


#ifdef __cplusplus
extern "C" {
#endif
extern void func_00153cc4(struct StreamPlayer *player, const struct StreamPlaybackInfo *info);
extern int func_001533e0(const struct PrefetchFileReader *reader, struct PrefetchChannelInfo *output, unsigned index);
extern const void *func_00153f90(const struct PrefetchChannelEntry *entry);
extern unsigned func_0017e184(unsigned encoding);
extern void func_0015475c(struct StreamChannel *stream, struct RecordState *block, int update);
#ifdef __cplusplus
}
#endif
#endif
