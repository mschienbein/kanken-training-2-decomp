#include <preinclude.h>
#include "nw/snd/snd_WaveArchiveFileReader.h"

bool nw::snd::internal::WaveArchiveFileReader::HasIndividualLoadTable() const
{
    if (m_Initialized) {
        const WaveArchiveFile::FileHeader *file = m_pHeader;
        unsigned offset = file->GetFileBlockOffset();
        /* S15: the type-0x6801 reference selects a real serialized block header;
         * CWAT distinguishes its individual-load table payload. */
        const void *address = static_cast<const unsigned char *>(
            static_cast<const void *>(file)) + static_cast<int>(offset);
        const nw::ut::BinaryBlockHeader *block =
            static_cast<const nw::ut::BinaryBlockHeader *>(address);
        if (block->kind == 0x54415743)
            return true;
    }
    return false;
}
