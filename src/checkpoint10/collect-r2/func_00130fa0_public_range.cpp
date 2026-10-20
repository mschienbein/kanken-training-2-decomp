/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr, pinned commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd; existing project LICENSE.txt.
 * Source: https://github.com/ctrdecomp/NintendoWare4Ctr/blob/15ddb5632db5a6f0d2f46e3827fc78df468684bd/sources/libraries/snd/snd_GroupFileReader.cpp
 * Selected constructor and validation helper; release assertions omitted.
 */
#include <preinclude.h>
#include <nw/snd/snd_GroupFileReader.h>

namespace nw { namespace snd { namespace internal {
namespace {
bool IsValidFileHeader(const void *groupFile)
{
    const ut::BinaryFileHeader *header = reinterpret_cast<const ut::BinaryFileHeader *>(groupFile);
    if (header->signature != GroupFileReader::SIGNATURE_FILE)
        return false;
    if (header->version - 0x01000000u > 0x10000u)
        return false;
    return true;
}
}
GroupFileReader::GroupFileReader(const void *groupFile)
    : m_pInfoBlockBody(NULL), m_pFileBlockBody(NULL), m_pInfoExBlockBody(NULL)
{
    if (!IsValidFileHeader(groupFile))
        return;
    const GroupFile::FileHeader *header = static_cast<const GroupFile::FileHeader *>(groupFile);
    const GroupFile::InfoBlock *info = header->GetInfoBlock();
    const GroupFile::FileBlock *files = header->GetFileBlock();
    const GroupFile::InfoExBlock *extra = header->GetInfoExBlock();
    if (info == NULL)
        return;
    if (files == NULL)
        return;
    m_pInfoBlockBody = &info->body;
    m_pFileBlockBody = &files->body;
    if (extra)
        m_pInfoExBlockBody = &extra->body;
}
} } }
