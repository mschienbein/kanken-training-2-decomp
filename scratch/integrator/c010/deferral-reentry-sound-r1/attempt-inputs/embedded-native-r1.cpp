#include "game/sound_task.h"

/* [INFERENCE] The original construction at 0014b824 and the shared concrete
 * type table establish this owner/member lifetime, not the old WaveSound view.
 * Descriptive C++ names do not assert an unrecovered historical mangling. */
SoundStreamPlayer::~SoundStreamPlayer()
{
    control_vtable = data_00224a34;
    base.vtable = data_00224a00;
    parameter_vtable = data_00224a48;
    func_001d8d7c(this);
    base.vtable = data_00224940;
}

EmbeddedSoundInstance::~EmbeddedSoundInstance()
{
    base.vtable = data_002246b8;
}
