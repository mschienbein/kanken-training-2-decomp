/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_SequenceSoundFile.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SEQUENCE_SOUND_FILE_H_
#define NW_SND_SEQUENCE_SOUND_FILE_H_

#include "../ut/ut_BinaryFileFormat.h"
#include "snd_Util.h"

namespace nw {
namespace snd {
namespace internal {

struct SequenceSoundFile
{
    struct DataBlock;
    struct LabelBlock;

    struct FileHeader : public Util::SoundFileHeader
    {
        const DataBlock* GetDataBlock() const;
        const LabelBlock* GetLabelBlock() const;
    };

    struct DataBlockBody
    {
        u8 sequenceData[1];
        const void* GetSequenceData() const { return reinterpret_cast<const void*>(sequenceData); }
    };

    struct DataBlock
    {
        ut::BinaryBlockHeader header;
        DataBlockBody body;
    };

    struct LabelInfo;
    struct LabelBlockBody
    {
        Util::ReferenceTable labelInfoReferenceTable;

        inline int GetLabelCount() const { return labelInfoReferenceTable.count; }
        const LabelInfo* GetLabelInfo(int index) const;
        const char* GetLabel(int index) const;
        const char* GetLabelByOffset(u32 offset) const;
        bool GetOffset(int index, u32* offsetPtr) const;
        bool GetOffsetByLabel(const char* label, u32* offsetPtr) const;
    };

    struct LabelBlock
    {
        ut::BinaryBlockHeader header;
        LabelBlockBody body;
    };

    struct LabelInfo
    {
        Util::Reference referToSequenceData;
        ut::ResU32 labelStringLength;
        char label[1];
    };

};

} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_SEQUENCE_SOUND_FILE_H_

