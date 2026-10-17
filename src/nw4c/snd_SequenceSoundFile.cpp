/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_SequenceSoundFile.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_SequenceSoundFile.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_SequenceSoundFile.h>
#include <cstring>      // strlen, strncmp

namespace nw {
namespace snd {
namespace internal {

/* SequenceSoundFile::FileHeader */

const SequenceSoundFile::DataBlock* SequenceSoundFile::FileHeader::GetDataBlock() const
{
    return reinterpret_cast<const DataBlock*>(GetBlock(20480));
}
const SequenceSoundFile::LabelBlock* SequenceSoundFile::FileHeader::GetLabelBlock() const
{
    return reinterpret_cast<const LabelBlock*>(GetBlock(20481));
}

/* SequenceSoundFile::LabelBlockBody */



} // namespace internal
} // namespace snd
} // namespace nw

