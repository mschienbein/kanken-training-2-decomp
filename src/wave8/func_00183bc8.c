#include "game/archive.h"

const void *func_00183bc8(const struct ArchiveReader *reader, unsigned index,
                          const void *replacement)
{
    if (reader->initialized && reader->cache) {
        if (index < reader->info->count) {
            const void *previous = reader->cache[index];
            reader->cache[index] = replacement;
            return previous;
        }
    }
    return 0;
}
