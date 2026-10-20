#include "game/archive.h"

/* Unobserved virtual slots describe layout only; their signatures are not recovered or invoked here. */

bool func_00185b44(const SoundArchive *archive, unsigned sound_id, SoundType1Info *out)
{
    bool result = func_00139358(archive->reader, sound_id, out);
    if (archive->addon) {
        result |= archive->addon->read_sequence(sound_id, out);
    }
    return result;
}
