/* SPDX-License-Identifier: GPL-3.0-only
 * Continues the preserved retail audio reconstruction. Historical command
 * identity and translation-unit membership remain [INFERENCE]. */
#include <preinclude.h>
#include "game/sound_runtime.h"
#include "game/stream_manager.h"

struct SequenceAuxiliaryEvent : StreamEvent {
    SequenceStartOptions options;
    const void *data;
};

static StreamEvent *allocate_command(StreamManager *manager, unsigned words)
{
    StreamEvent *event = static_cast<StreamEvent *>(func_0019147c(manager, words));
    event->start = manager->write_index;
    return event;
}

void func_001d2f90(SequenceSound *sound, const void *data, const SequenceStartOptions *options)
{
    StreamManager *manager = func_001916e8();
    SequenceAuxiliaryEvent *event = static_cast<SequenceAuxiliaryEvent *>(allocate_command(manager, 10));
    event->opcode = 0x29;
    event->data = data;
    event->object = &sound->playback;
    event->options = *options;
    func_00191458(manager, event);
}
