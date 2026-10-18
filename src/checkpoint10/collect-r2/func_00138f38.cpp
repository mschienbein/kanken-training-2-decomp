#include "preinclude.h"
#include "nw/snd/snd_WaveSoundFile.h"

namespace nw { namespace snd { namespace internal {

const WaveSoundFile::InfoBlock *WaveSoundFile::FileHeader::GetInfoBlock() const
{
    /* [INFERENCE] CWSD INFO-block type recovered from the block-table lookup. */
    return static_cast<const InfoBlock *>(GetBlock(0x6800));
}

} } }
