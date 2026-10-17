#include <preinclude.h>
#include "game/archive.h"
/* SPDX-License-Identifier: GPL-3.0-only
 * LoadFile/LoadImpl control flow adapted from public NintendoWare4Ctr,
 * sources/libraries/snd/snd_SoundArchiveLoader.cpp, commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd. Selected definitions only;
 * address-named callees preserve the currently evidenced image contracts.
 */
#include <nw/snd/snd_SoundArchiveLoader.h>
#include <nw/snd/snd_SoundMemoryAllocatable.h>
#include "game/functions.h"

/* INTEGRATOR: move to include/game/sound_archive_loader.h */
extern "C" {

int func_001839cc(nw::snd::internal::SoundArchiveLoader *, unsigned, void *,
                       unsigned, int, unsigned);
unsigned func_00183030(void *, unsigned);
}

namespace nw { namespace snd { namespace internal {
static inline bool loader_device_memory(uptr memory, size_t size)
{
    const uptr head = func_0019d058();
    const uptr tail = head + func_0019d048();
    /* ORIGINAL BUG: unsigned end-address additions may wrap. */
    return head <= memory && memory + size <= tail;
}

inline void *SoundArchiveLoader::LoadFile(SoundArchive::FileId fileId,
    SoundMemoryAllocatable *allocator, size_t loadBlockSize, bool needDeviceMemory)
{
    SoundArchive::FileInfo info;
    if (!func_00138edc(m_pSoundArchive, fileId, &info))
        return NULL;
    u32 fileSize = info.fileSize;
    if (fileSize == 0 || fileSize == 0xffffffff)
        return NULL;
    void *buffer = allocator->Alloc(fileSize);
    if (buffer == NULL)
        return NULL;
    if (needDeviceMemory && !loader_device_memory(reinterpret_cast<uptr>(buffer), fileSize))
        return NULL;
    if (func_001839cc(this, fileId, buffer, fileSize, 0, loadBlockSize) != fileSize)
        return NULL;
    func_00183030(buffer, fileSize);
    return buffer;
}

const void *SoundArchiveLoader::LoadImpl(SoundArchive::FileId fileId,
    SoundMemoryAllocatable *allocator, size_t loadBlockSize, bool needDeviceMemory)
{
    const void *address = GetFileAddressImpl(fileId);
    if (address == NULL) {
        address = LoadFile(fileId, allocator, loadBlockSize, needDeviceMemory);
        if (address == NULL) {
            return NULL;
        } else {
            SetFileAddressToTable(fileId, address);
            return address;
        }
    } else {
        return address;
    }
}
} } }
