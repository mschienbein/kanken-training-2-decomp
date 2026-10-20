#include "game/archive.h"

unsigned func_00140dd4(const ArchiveReader *reader, int index)
{
    if (!reader->initialized)
        return 0;
    const unsigned file_offset = func_00183c54(reader->file);
    const ArchiveInfoTable *info = reader->info;
    return file_offset + sizeof(ArchiveBlockHeader)
        + info->GetOffsetFromFileBlockBody(static_cast<unsigned>(index));
}
