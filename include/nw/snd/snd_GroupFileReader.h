/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_GroupFileReader.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_GROUP_FILE_READER_H_
#define NW_SND_GROUP_FILE_READER_H_

#include "snd_GroupFile.h"

namespace nw {
namespace snd {
namespace internal {

struct GroupItemLocationInfo
{
    u32 fileId;
    const void* address;
};

class GroupFileReader
{
public:
    static const u32 SIGNATURE_FILE = NW_UT_MAKE_SIGWORD('C', 'G', 'R', 'P');

    GroupFileReader(const void* groupFile);

    u32 GetGroupItemCount() const { return m_pInfoBlockBody->GetGroupItemInfoCount(); }
    bool ReadGroupItemLocationInfo( GroupItemLocationInfo* out, u32 index ) const;

    u32 GetGroupItemExCount() const;
    bool ReadGroupItemInfoEx(GroupFile::GroupItemInfoEx* out, u32 index) const;

private:
    const GroupFile::InfoBlockBody* m_pInfoBlockBody;
    const GroupFile::FileBlockBody* m_pFileBlockBody;
    const GroupFile::InfoExBlockBody* m_pInfoExBlockBody;
};

} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_GROUP_FILE_READER_H_

