#include "game/archive.h"

/* Current catalogue records the adjacent reader wrapper as its own function. */
extern unsigned func_00131950(const struct SoundArchiveReader *reader, unsigned sound_id);

unsigned func_00131948(const struct SoundArchive *archive, unsigned sound_id)
{
    return func_00131950(archive->reader, sound_id);
}
