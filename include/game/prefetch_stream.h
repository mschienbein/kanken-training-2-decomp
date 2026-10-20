#ifndef GAME_PREFETCH_STREAM_H
#define GAME_PREFETCH_STREAM_H
#include "game/audio_voice.h"

struct PrefetchBlockReference {
    unsigned short type, reserved02;
    int offset;
    unsigned size;
#ifdef __cplusplus
    bool IsValidTypeId(unsigned short valid) const
    {
        if (valid == type)
            return true;
        return false;
    }
#endif
};
#ifdef __cplusplus
/* GPL-3.0-only: algorithm from pinned NW4C Util::BlockReferenceTable.
 * ORIGINAL BUG: the serialized one-element tail is indexed beyond its C++
 * bound; the surrounding file's count establishes available storage. */
struct PrefetchBlockReferenceTable {
    PrefetchBlockReference item[1];
    const PrefetchBlockReference *GetReference(unsigned short type,
                                              unsigned short count) const
    {
        for (int i = 0; i < count; ++i) {
            if (item[i].IsValidTypeId(type))
                return &item[i];
        }
        return 0;
    }
    const void *GetReferedItem(const void *origin, unsigned short type,
                              unsigned short count) const
    {
        const PrefetchBlockReference *reference = GetReference(type, count);
        if (reference == 0)
            return 0;
        if (reference->offset == 0)
            return 0;
        return static_cast<const unsigned char *>(origin) + reference->offset;
    }
};
#endif
/* Serialized CSPT header; the native C++ prefix is not the complete variable
 * file extent. C clients describe the same storage using a flexible tail. */
struct PrefetchFileHeader {
    unsigned signature;
    unsigned short byte_order, header_size;
    unsigned version, file_size;
    unsigned short block_count, reserved12;
#ifdef __cplusplus
    PrefetchBlockReferenceTable blockReferenceTable;
    const void *GetBlock(unsigned short type) const
    {
        return blockReferenceTable.GetReferedItem(this, type, block_count);
    }
#else
    struct PrefetchBlockReference blocks[];
#endif
};
struct PrefetchInfoBlock;
struct PrefetchFileBlock {
    unsigned signature;
    unsigned size;
};

struct PrefetchChannelEntry { unsigned start, size, reserved08[2]; int data_offset; };
struct PrefetchDataBlock {
    unsigned count;
#ifndef __cplusplus
    struct PrefetchChannelEntry entries[];
#endif
};

struct AdpcmLoopState {
    short predictor_scale;
    short history1;
    short history2;
};
struct AdpcmChannelParameters {
    short coefficients[16];
    short predictor_scale;
    short history1;
    short history2;
};
struct StreamChannel {
    void *allocation;
    struct AudioVoiceGroup *channel;
    struct RecordState blocks[32];
    struct AdpcmLoopState loop_states[32];
};
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
struct PrefetchChannelReference {
    unsigned short type, reserved02;
    int offset;
};
struct PrefetchReferenceTable {
    unsigned count;
#ifndef __cplusplus
    struct PrefetchChannelReference entries[];
#endif
};
struct StreamBufferState {
    unsigned request_id;
    unsigned context;
};
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

#ifdef __cplusplus
struct AudioReadStream;
/* [INFERENCE] Decoder fields and command roles describe target consumers,
 * not the complete public/native stream class. */
struct AudioDecodeState {
    AudioReadStream *stream;
    unsigned reserved04;
    unsigned read_base;
    unsigned short read_stride, reserved0e;
    unsigned resource;
    void *source;
    StreamPlaybackInfo *info;
    unsigned char kind, reserved1d[3];
    unsigned channels;
    unsigned short mode;
    unsigned char looping, reset_region;
    unsigned loop_start, loop_end;
    int (*advance)(unsigned *block, void *context);
    void *context;
    unsigned reserved38, last_block, first_block, reserved44[2], first_sample;
    unsigned char reserved50, finished, reserved52[2];
    unsigned block, position, begin, end;
    unsigned char reserved64[0xb94 - 0x64];
    unsigned char encoding, reservedb95[0xcf8 - 0xb95];
    unsigned adpcm_begin;
};
struct AudioDecodeEvent : StreamEvent {
    bool success;
    unsigned char reserved15[3];
    unsigned channel_offsets[8];
    unsigned short mode;
    unsigned char reserved3a[2];
};
struct AudioDecodeOwner {
    unsigned char reserved00[0x1c];
    AudioDecodeState *decoder;
};
extern "C" {
/* [INFERENCE] The original consumer stores the decoder's boolean result in
 * the command success byte; complete decoder/native identities are unrecovered. */
bool func_001d60dc(AudioDecodeState *decoder, AudioDecodeEvent *event);
bool func_001d63fc(AudioDecodeState *decoder, AudioDecodeEvent *event);
unsigned func_001d68c0(AudioDecodeOwner *owner);
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern const struct PrefetchFileBlock *func_00153ebc(const struct PrefetchFileHeader *file);
extern const struct PrefetchFileBlock *func_00153f28(const struct PrefetchFileHeader *file);
extern void func_001528dc(struct PrefetchFileReader *reader, const struct PrefetchFileHeader *file);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
extern void func_00153cc4(struct StreamPlayer *player, const struct StreamPlaybackInfo *info);
extern int func_001533e0(const struct PrefetchFileReader *reader, struct PrefetchChannelInfo *output, unsigned index);
extern const void *func_00153f90(const struct PrefetchChannelEntry *entry);
extern unsigned func_0017e184(unsigned encoding);
extern void func_0015475c(struct StreamChannel *stream, struct RecordState *block, int update);
extern const struct PrefetchChannelReference *func_00154804(
    const struct PrefetchReferenceTable *table, unsigned index);
#ifdef __cplusplus
}
#endif
#endif
