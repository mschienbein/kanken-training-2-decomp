/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_WaveFile.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */

#ifndef NW_SND_WAVE_FILE_H_
#define NW_SND_WAVE_FILE_H_

#include "../../nn/types.h"

#include "snd_Global.h"
#include "snd_Util.h"
#include "../ut/ut_BinaryFileFormat.h"
#include "../ut/ut_ResTypes.h"

namespace nw {
namespace snd {
namespace internal {

struct WaveFile
{
    enum EncodeMethod
    {
        PCM8,
        PCM16,
        DSP_ADPCM,
        IMA_ADPCM
    };

    struct InfoBlock;
    struct ChannelInfo;
    struct DspAdpcmInfo;
    struct DataBlock;

    struct FileHeader : public Util::SoundFileHeader
    {
        const InfoBlock* GetInfoBlock() const;
        const DataBlock* GetDataBlock() const;
    };

    struct InfoBlockBody
    {
        u8 encoding;
        u8 isLoop;
        u16 padding;
        nw::ut::ResU32 sampleRate;
        nw::ut::ResU32 loopStartFrame;
        nw::ut::ResU32 loopEndFrame;
        nw::ut::ResU32 reserved;
        Util::ReferenceTable channelInfoReferenceTable;

        inline s32 GetChannelCount() const { return channelInfoReferenceTable.count; }
        const ChannelInfo& GetChannelInfo( s32 channelIndex ) const;
    };

    struct InfoBlock
    {
        ut::BinaryBlockHeader   header;
        InfoBlockBody           body;
    };

    struct ChannelInfo
    {
        Util::Reference     referToSamples;
        Util::Reference     referToAdpcmInfo;
        nw::ut::ResU32      reserved;

        const void* GetSamplesAddress(const void* dataBlockBodyAddress) const;
        const DspAdpcmInfo& GetDspAdpcmInfo() const;
    };

    struct DspAdpcmInfo
    {
        DspAdpcmParam      adpcmParam;
        DspAdpcmLoopParam  adpcmLoopParam;
    };

    struct DataBlock
    {
        ut::BinaryBlockHeader   header;
        union
        {
            s8  pcm8[1];
            s16 pcm16[1];
            u8  byte[1];
        };
    };
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_WAVE_FILE_H_

