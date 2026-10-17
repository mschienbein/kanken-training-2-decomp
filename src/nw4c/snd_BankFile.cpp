/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_BankFile.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_BankFile.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_BankFile.h>

namespace nw {
namespace snd {
namespace internal {

// FileHeader

const BankFile::InfoBlock* BankFile::FileHeader::GetInfoBlock() const
{
    return (InfoBlock*) Util::SoundFileHeader::GetBlock(0x5800);
}

}
}
}

