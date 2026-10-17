#define NW_RELEASE
#include "game/archive.h"
#include <nw/snd/snd_WaveArchiveFile.h>

/* INTEGRATOR: move to include/game/archive.h. ArchiveInfoTable exposes only
 * the count prefix; the shared NW4C payload supplies its reference records. */
extern "C" {
unsigned func_00140dd4(const ArchiveReader *reader, int index);
}

unsigned func_00140dd4(const ArchiveReader *reader, int index)
{
    if (!reader->initialized)
        return 0;
    const unsigned file_offset = func_00183c54(reader->file);
    const nw::snd::internal::WaveArchiveFile::InfoBlockBody *info =
        reinterpret_cast<const nw::snd::internal::WaveArchiveFile::InfoBlockBody *>(reader->info);
    return file_offset + sizeof(nw::ut::BinaryBlockHeader)
        + info->GetOffsetFromFileBlockBody(static_cast<u32>(index));
}
