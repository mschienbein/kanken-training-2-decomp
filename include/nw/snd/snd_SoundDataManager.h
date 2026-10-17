/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_SoundDataManager.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SOUND_DATA_MANAGER_H_
#define NW_SND_SOUND_DATA_MANAGER_H_

#include "snd_DisposeCallback.h"
#include "snd_SoundArchive.h"
#include "snd_SoundArchiveLoader.h"
#include "snd_Util.h"

namespace nw { 
namespace snd {

class SoundDataManager;

namespace internal {

class SoundFileManager
{
public:
    virtual const void* GetFileAddress(SoundArchive::FileId fileId) const = 0;
    virtual const void* GetFileWaveDataAddress(SoundArchive::FileId fileId) const = 0;
};

}

class SoundDataManager : public internal::driver::DisposeCallback, public internal::SoundArchiveLoader
{
public:
    SoundDataManager();
    virtual ~SoundDataManager();

    size_t GetRequiredMemSize(const SoundArchive* arc) const;
    bool Initialize(const SoundArchive* arc, void* buffer, u32 size);
    void Finalize();

    void detail_SetFileManager(internal::SoundFileManager* fileManager)
    {
        m_pFileManager = fileManager;
    }

    const void* SetFileAddress(SoundArchive::FileId fileId, const void* address);
    const void* detail_GetFileAddress(SoundArchive::FileId fileId) const;
    u32 detail_GetFileIdFromTable(const void* address) const;
protected:
    virtual void InvalidateData(const void* start, const void* end);
    virtual const void* SetFileAddressToTable(SoundArchive::FileId fileId, const void* address );
    virtual const void* GetFileAddressFromTable(SoundArchive::FileId fileId ) const;
    virtual const void* GetFileAddressImpl(SoundArchive::FileId fileId ) const;

private:
    struct FileAddress
    {
        const void* address;
    };

    typedef internal::Util::Table<FileAddress> FileTable;
    bool CreateFileAddressTable(const SoundArchive* arc, void** buffer, void* endp);

    FileTable* m_pFileTable;
    internal::SoundFileManager* m_pFileManager;
};

} // namespace snd
} // namespace nw

#endif // NW_SND_SOUND_DATA_MANAGER_H_

