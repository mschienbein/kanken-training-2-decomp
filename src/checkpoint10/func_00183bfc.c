#include "game/archive.h"

struct ArchiveDataSpan { const unsigned char *data; };

static struct ArchiveDataSpan file_data(const struct ArchiveFile *file)
{
    struct ArchiveDataSpan result;
    result.data = (const unsigned char *)(func_001440b8(file) + 1);
    return result;
}

const void *func_00183bfc(const struct ArchiveReader *reader, unsigned index)
{
    const struct ArchiveInfoTable *table;
    if (!reader->initialized)
        return 0;
    table = reader->info;
    if (index >= table->count)
        return 0;
    if (reader->cache)
        return reader->cache[index];
    {
        unsigned offset = table->entries[index].offset;
        return file_data(reader->file).data + offset;
    }
}
