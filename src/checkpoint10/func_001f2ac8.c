#include "game/archive.h"

const void *func_001f2ac8(const struct SoundArchiveReader *reader, unsigned sound_id)
{
    if (reader->lookup_reference) {
        unsigned index = func_001f206c(reader->info, sound_id);
        if (index == ~0u)
            return 0;
        return func_001f2308(reader->lookup_reference, index);
    }
    return 0;
}
