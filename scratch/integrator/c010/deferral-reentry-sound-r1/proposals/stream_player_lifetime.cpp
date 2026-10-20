#include "game/sound_task.h"

/* [INFERENCE] Target-version player lifetime from its construction, original
 * cleanup fields, stop receiver and pointer-preserving return. */
SoundStreamPlayer::~SoundStreamPlayer()
{
    base.vtable = &data_00224a00;
    control_vtable = data_00224a00.control;
    parameter_vtable = data_00224a00.parameter;
    func_001d8d7c(this);
    base.vtable = data_00224940;
}
