#include "game/archive.h"





struct SoundWaveReader *func_001d3c78(struct SoundWaveReader *reader,
                                    const struct SoundWaveFile *file, unsigned char relocated)
{
    reader->file = 0;
    reader->info = 0;
    if (!(reader->relocated = relocated) && file->signature == 0x56415743 &&
        file->version - 0x02000000u <= 0x00010100u) {
        const struct ArchiveBlockHeader *info;
        const struct ArchiveBlockHeader *data;
        reader->file = file;
        info = func_001f481c(reader->file);
        data = func_001f47b4(reader->file);
        if (!info)
            return reader;
        if (!data)
            return reader;
        if (info->signature != 0x4f464e49)
            return reader;
        if (data->signature != 0x41544144)
            return reader;
        reader->data = data + 1;
        reader->info = (const struct SoundWaveInfo *)(info + 1);
    }
    return reader;
}
