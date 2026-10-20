#include "game/archive.h"

/* Current catalogue records the adjacent reader wrapper as its own function. */
#include "game/sound_archive.h"

unsigned func_00131948(const struct SoundArchive *archive, unsigned sound_id)
{
    return func_00131950(archive->reader, sound_id);
}
