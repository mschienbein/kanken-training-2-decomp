/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_SequenceSoundFileReader.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SEQUENCE_SOUND_FILE_READER_H_
#define NW_SND_SEQUENCE_SOUND_FILE_READER_H_

#include "snd_SequenceSoundFile.h"

namespace nw {
namespace snd {
namespace internal {

class SequenceSoundFileReader
{
public:
    static const u32 SIGNATURE_FILE = NW_UT_MAKE_SIGWORD('C', 'S', 'E', 'Q');

    SequenceSoundFileReader(const void* sequenceFile);
    bool IsAvailable() const { return m_pHeader != NULL; }

    const void* GetSequenceData() const;
    bool GetOffsetByLabel(const char* label, u32* offsetPtr) const;
    const char* GetLabelByOffset(u32 offset) const;

    inline int GetLabelCount() const
    {
        return m_pLabelBlockBody->GetLabelCount();
    }
    inline const char* GetLabel(int index) const
    {
        return m_pLabelBlockBody->GetLabel(index);
    }

private:
    const SequenceSoundFile::FileHeader* m_pHeader;
    const SequenceSoundFile::DataBlockBody* m_pDataBlockBody;
    const SequenceSoundFile::LabelBlockBody* m_pLabelBlockBody;
};

} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_SEQUENCE_SOUND_FILE_READER_H_

