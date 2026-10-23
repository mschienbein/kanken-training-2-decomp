/* [INFERENCE] Canonical BasicSound supplies the proven virtual interface here.
 * This method does not establish the full historical object-field layout. */
/* SPDX-License-Identifier: GPL-3.0-only
 * Native wrapper structure adapted from NintendoWare4Ctr snd_BasicSound.cpp,
 * pinned 15ddb5632db5a6f0d2f46e3827fc78df468684bd.  The command manager uses
 * the project's recovered address-named StreamManager interface. */
#include <preinclude.h>
#include "nw/snd/snd_BasicSound.h"
#include "game/stream_manager.h"

struct NativeFrontBypassEvent : StreamEvent {
    bool value;
};

template <class Command>
static Command *allocate_command(StreamManager *manager)
{
    Command *command = static_cast<Command *>(func_0019147c(manager, (sizeof(Command) + 3) / 4));
    command->start = manager->write_index;
    return command;
}

namespace nw { namespace snd { namespace internal {
void BasicSound::SetFrontBypass(bool value)
{
    StreamManager *manager = func_001916e8();
    NativeFrontBypassEvent *event = allocate_command<NativeFrontBypassEvent>(manager);
    event->opcode = 11;
    event->object = GetBasicSoundPlayerHandle();
    event->value = value;
    func_00191458(manager, event);
}
} } }
