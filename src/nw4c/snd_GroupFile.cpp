/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_GroupFile.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_GroupFile.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_GroupFile.h>

namespace nw {
namespace snd {
namespace internal {

// FileHeader

const GroupFile::InfoBlock* GroupFile::FileHeader::GetInfoBlock() const
{
    return (InfoBlock*)Util::SoundFileHeader::GetBlock(0x7800);
}

const GroupFile::FileBlock* GroupFile::FileHeader::GetFileBlock() const
{
    return (FileBlock*)Util::SoundFileHeader::GetBlock(0x7801);
}

const GroupFile::InfoExBlock* GroupFile::FileHeader::GetInfoExBlock() const
{
    return (InfoExBlock*)Util::SoundFileHeader::GetBlock(0x7802);
}

} // namespace internal
} // namespace snd
} // namespace nw

