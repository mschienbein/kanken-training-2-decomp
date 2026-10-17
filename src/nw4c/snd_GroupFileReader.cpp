/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_GroupFileReader.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_GroupFileReader.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_GroupFileReader.h>

namespace nw {
namespace snd {
namespace internal {
namespace 
{
const u32 SIGNATURE_INFO_BLOCK  = NW_UT_MAKE_SIGWORD( 'I', 'N', 'F', 'O' );
const u32 SIGNATURE_FILE_BLOCK  = NW_UT_MAKE_SIGWORD( 'F', 'I', 'L', 'E' );
const u32 SIGNATURE_INFOEX_BLOCK  = NW_UT_MAKE_SIGWORD( 'I', 'N', 'F', 'X' );

const u32 SUPPORTED_FILE_VERSION = 0x01000000;
const u32 CURRENT_FILE_VERSION   = 0x01010000;

} // namespace

bool GroupFileReader::ReadGroupItemLocationInfo(GroupItemLocationInfo* out, u32 index) const
{
    if (m_pInfoBlockBody == NULL)
    {
        return false;
    }

    const GroupFile::GroupItemInfo* groupItemInfo = m_pInfoBlockBody->GetGroupItemInfo(index);
    if (groupItemInfo == NULL)
    {
        return false;
    }

    out->fileId = groupItemInfo->fileId;
    out->address = groupItemInfo->GetFileLocation(m_pFileBlockBody);
    return true;
}

u32 GroupFileReader::GetGroupItemExCount() const
{
    if (m_pInfoExBlockBody == NULL)
    {
        return 0;
    }

    return m_pInfoExBlockBody->GetGroupItemInfoExCount();
}

bool GroupFileReader::ReadGroupItemInfoEx(GroupFile::GroupItemInfoEx* out, u32 index) const
{
    if (m_pInfoExBlockBody == NULL)
    {
        return false;
    }

    const GroupFile::GroupItemInfoEx* groupItemInfoEx = m_pInfoExBlockBody->GetGroupItemInfoEx(index);
    if (groupItemInfoEx == NULL)
    {
        return false;
    }

    *out = *groupItemInfoEx;
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw

