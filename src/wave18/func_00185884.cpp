#include "game/archive.h"

/* Unobserved virtual slots describe layout only; their signatures are not recovered or invoked here. */

bool func_00185884(const SoundArchive *archive, unsigned sound_id, SoundType3Info *out)
{
    bool result = func_001392b0(archive->reader, sound_id, out);
    if (archive->addon) {
        result |= archive->addon->read_wave_sound(sound_id, out);
    }
    return result;
}
