/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_WaveFile.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_WaveFile.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_WaveFile.h>

#include <nw/ut/ut_Inlines.h>

namespace nw {
namespace snd {
namespace internal {

/* WaveFile::FileHeader */

const WaveFile::InfoBlock* WaveFile::FileHeader::GetInfoBlock() const
{
    return reinterpret_cast<const InfoBlock*>(GetBlock(28672));
}

const WaveFile::DataBlock* WaveFile::FileHeader::GetDataBlock() const
{
    return reinterpret_cast<const DataBlock*>(GetBlock(28673));
}

/* WaveFile::InfoBlockBody */

const WaveFile::ChannelInfo& WaveFile::InfoBlockBody::GetChannelInfo(s32 channelIndex) const
{
    NW_ASSERT(channelIndex < GetChannelCount());
    return *reinterpret_cast<const ChannelInfo*>(channelInfoReferenceTable.GetReferedItem(channelIndex));
}

/* WaveFile::ChannelInfo */

const void* WaveFile::ChannelInfo::GetSamplesAddress(const void* dataBlockBodyAddress) const
{
    return reinterpret_cast<const void*>(
        ut::AddOffsetToPtr(dataBlockBodyAddress, referToSamples.offset));
}

} // namespace internal
} // namespace snd
} // namespace nw

