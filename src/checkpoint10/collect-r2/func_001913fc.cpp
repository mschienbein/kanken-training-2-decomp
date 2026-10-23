/* SPDX-License-Identifier: GPL-3.0-only
 * Native BasicSound/command types from the pinned NintendoWare4Ctr headers.
 * [INFERENCE] Same-address SetPanMode identity: the public member contract,
 * playback accessor and pan-mode command agree; original spelling is not proved.
 * Allocation follows the established address-named StreamManager interface. */
#include <preinclude.h>
#include "nw/snd/snd_BasicSound.h"
#include "game/stream_manager.h"

struct NativePanModeEvent : StreamEvent {
    nw::snd::PanMode mode;
    nw::snd::PanCurve curve;
};

template <class Command>
static Command *allocate_command(StreamManager *manager)
{
    Command *command = static_cast<Command *>(func_0019147c(manager, (sizeof(Command) + 3) / 4));
    command->start = manager->write_index;
    return command;
}

namespace nw { namespace snd { namespace internal {
void BasicSound::SetPanMode(PanMode mode)
{
    StreamManager *manager = func_001916e8();
    NativePanModeEvent *event = allocate_command<NativePanModeEvent>(manager);
    event->opcode = 9;
    event->object = GetBasicSoundPlayerHandle();
    event->mode = mode;
    func_00191458(manager, event);
}
} } }
