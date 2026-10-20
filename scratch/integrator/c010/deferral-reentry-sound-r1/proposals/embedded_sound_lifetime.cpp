#include "game/sound_task.h"

/* [INFERENCE] The original construction at 0014b824 and the shared concrete
 * type table establish this owner/member lifetime, not the old WaveSound view.
 * Descriptive C++ names do not assert an unrecovered historical mangling. */

EmbeddedSoundInstance::~EmbeddedSoundInstance()
{
    base.vtable = data_002246b8;
}
