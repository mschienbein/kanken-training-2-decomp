#include <preinclude.h>
#include "nw/snd/snd_WaveArchiveFile.h"

unsigned nw::snd::internal::WaveArchiveFile::FileHeader::GetFileBlockOffset() const
{
    const nw::snd::internal::Util::ReferenceWithSize *reference;
    unsigned short block_type = 0x6801;
    if (toBlocks[0].typeId == block_type)
        reference = &toBlocks[0];
    else if (toBlocks[1].typeId == block_type)
        reference = &toBlocks[1];
    else
        reference = 0;
    /* ORIGINAL BUG: a missing file block dereferences a null reference. */
    return reference->offset;
}
