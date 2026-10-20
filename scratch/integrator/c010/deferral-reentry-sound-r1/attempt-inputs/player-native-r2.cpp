#include "game/sound_task.h"

/* [INFERENCE] Target-version player lifetime from its construction, original
 * cleanup fields, stop receiver and pointer-preserving return. */
SoundStreamPlayer::~SoundStreamPlayer()
{
    control_vtable = data_00224a34;
    base.vtable = data_00224a00;
    parameter_vtable = data_00224a48;
    func_001d8d7c(this);
    base.vtable = data_00224940;
}
