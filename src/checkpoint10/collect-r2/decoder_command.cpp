/* SPDX-License-Identifier: GPL-3.0-only
 * step3: private retail audio reconstruction.
 * [INFERENCE] Address adjacency is not recovered historical TU membership.
 */
struct AudioReadStream;
#include "game/prefetch_stream.h"
#include "game/stream_manager.h"

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

/* INTEGRATOR: move the private C-linkage contracts into shared audio headers. */
extern "C" {
bool func_001d60dc(AudioDecodeState *, AudioDecodeEvent *);
bool func_001d63fc(AudioDecodeState *, AudioDecodeEvent *);
unsigned func_001d68c0(AudioDecodeOwner *);
}

static StreamEvent *allocate_event(StreamManager *manager, unsigned words)
{
    StreamEvent *event = static_cast<StreamEvent *>(func_0019147c(manager, words));
    event->start = manager->write_index;
    return event;
}

static unsigned publish_event(StreamManager *manager, StreamEvent *event, bool wait)
{
    unsigned serial = func_00191458(manager, event);
    if (wait)
        return func_001912b0(manager, 1);
    return serial;
}

unsigned func_001d68c0(AudioDecodeOwner *owner)
{
    AudioDecodeState *decoder = owner->decoder;
    StreamManager *manager = func_00185bfc();
    AudioDecodeEvent *event = static_cast<AudioDecodeEvent *>(allocate_event(manager, 15));
    event->opcode = 42;
    bool success;
    if (decoder->kind == 0)
        success = func_001d60dc(decoder, event);
    else if (decoder->kind == 1)
        success = func_001d63fc(decoder, event);
    else
        success = false;
    event->success = success;
    return publish_event(manager, event, true);
}
