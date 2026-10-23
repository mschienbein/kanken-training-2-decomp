#include <string.h>
#include "game/sound_archive_header.h"

static bool has_signature(const unsigned &actual, const unsigned &expected)
{
    return actual == expected;
}

void func_00147054(SoundArchiveHeader *destination, const SoundArchiveHeader *source)
{
    if (has_signature(source->signature, 0x52415343)) {
        if (source->version >= 0x02000000 && source->version <= 0x02030200)
            memcpy(destination, source, sizeof(*source));
    }
}
