#include "game/archive.h"
#include "game/sound_archive.h"

#include "game/archive_queries.h"

int func_00131950(const struct SoundArchiveReader *reader, unsigned int sound_id)
{
    return func_00139174(reader->info, sound_id);
}
