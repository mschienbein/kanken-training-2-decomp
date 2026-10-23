/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_SoundArchiveLoader.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain the embedded-group wave-table setup method only;
 * reuse canonical native headers; preserve the original missing-file behavior.
 * Modified 2026-10-20: return the archive buffer from its signature-append operation.
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
#include <nw/snd/snd_SoundArchiveLoader.h>
#include <nw/snd/snd_SoundMemoryAllocatable.h>
#include "game/archive.h"



namespace {
/* [INFERENCE] Source-level operation boundary; historical helper identity unknown.
 * Return the same archive buffer for subsequent reader construction and publication. */
static void* append_table_signature(void *file, u32 offset)
{
    std::memcpy(nw::ut::AddOffsetToPtr(file, offset),
        &data_00220b58, sizeof(data_00220b58));
    return file;
}
}

namespace nw { namespace snd { namespace internal {

/* [INFERENCE] Same-address native self-definition; the exact historical symbol
 * and original TU extent are not established by the address-based catalogue. */
void SoundArchiveLoader::SetWaveArchiveTableInEmbeddedGroupImpl(
    SoundArchive::ItemId warcId, SoundMemoryAllocatable* pAllocator)
{
    SoundArchive::WaveArchiveInfo info;
    if (!m_pSoundArchive->ReadWaveArchiveInfo(warcId, &info))
        return;
    if (!info.isLoadIndividual)
        return;

    /* [INFERENCE] The image uses native vtable slot +0x10, GetFileAddressImpl;
     * the pinned public revision instead spells the +0x0c table accessor. */
    const ArchiveFile* warcFile =
        static_cast<const ArchiveFile*>(GetFileAddressImpl(info.fileId));
    ArchiveReader loadedFileReader;
    func_00183c84(&loadedFileReader, warcFile, false);
    if (func_001391d8(&loadedFileReader) != 0)
        return;

    /* ORIGINAL BUG: a missing archive still reaches its file-block accessor. */
    u32 fileBlockOffset = func_00183c54(warcFile);
    const u32 requiredSize = fileBlockOffset + info.waveCount * sizeof(u32) +
        sizeof(data_00220b58);
    void* buffer = pAllocator->Alloc(requiredSize);
    if (buffer == NULL)
        return;

    std::memcpy(buffer, warcFile, fileBlockOffset);
    buffer = append_table_signature(buffer, fileBlockOffset);
    ArchiveReader reader;
    func_00183c84(&reader, static_cast<const ArchiveFile*>(buffer), true);
    func_00140970(&reader);
    for (u32 i = 0; i < info.waveCount; ++i)
        func_00183bc8(&reader, i, func_00183bfc(&loadedFileReader, i));
    SetFileAddressToTable(info.fileId, buffer);
}

} } }
