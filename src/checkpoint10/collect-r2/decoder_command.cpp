/* SPDX-License-Identifier: GPL-3.0-only
 * step3: private retail audio reconstruction.
 * [INFERENCE] Address adjacency is not recovered historical TU membership.
 */
#include "game/prefetch_stream.h"
#include "game/stream_manager.h"


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
