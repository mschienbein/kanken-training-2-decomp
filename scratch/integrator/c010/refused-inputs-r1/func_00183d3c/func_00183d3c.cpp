/* Adapted from NintendoWare4Ctr (GPL-3.0-only), sources/libraries/snd/snd_SoundArchiveLoader.cpp,
 * pinned 15ddb5632db5a6f0d2f46e3827fc78df468684bd: SoundArchiveLoader::LoadIndividualWave.
 * The target's NW4C build differs from the pinned header (see the INTEGRATOR notes below), so the
 * class views this function needs are declared locally instead of including include/nw/snd.
 * [INFERENCE] The target looks the wave archive file up through the third virtual slot
 * (GetFileAddressImpl), not GetFileAddressFromTable as the pinned source does. */
#include <stddef.h>
#include <string.h>

typedef unsigned int u32;
typedef int s32;
typedef unsigned char u8;
typedef unsigned int uptr;

namespace nn { namespace os {
uptr GetDeviceMemoryAddress();
size_t GetDeviceMemorySize();
} }
namespace nn { namespace snd { namespace CTR {
void FlushDataCache(uptr address, size_t size);
} } }

namespace nw {
namespace snd {

class SoundMemoryAllocatable
{
public:
    virtual ~SoundMemoryAllocatable() {}
    virtual void* Alloc(size_t size) = 0;
};

class SoundArchive
{
public:
    typedef u32 ItemId;
    typedef u32 FileId;
    u32 GetItemFileId(ItemId itemId) const;
};

namespace internal {

/* INTEGRATOR: differs from include/nw/snd/snd_WaveArchiveFileReader.h: the target's reader is 16 bytes with an
 * individual-load flag at +0xc that IsLoaded tests before GetWaveFile (reader on the stack at sp+8, flag read
 * from sp+0x14). */
class WaveArchiveFileReader
{
public:
    WaveArchiveFileReader(const void* pWaveArchiveFile, bool isIndividual = false);
    u32 GetWaveFileSize(u32 waveIndex) const;
    u32 GetWaveFileOffsetFromFileHead(u32 waveIndex) const;
    const void* GetWaveFile(u32 waveIndex) const;
    const void* SetWaveFile(u32 waveIndex, const void* pWaveFile);
    bool IsLoaded(u32 waveIndex) const
    {
        if (m_IsIndividual)
        {
            if (GetWaveFile(waveIndex) != NULL)
            {
                return true;
            }
        }
        return false;
    }

private:
    const void* m_pHeader;
    const void* m_pInfoBlockBody;
    void* m_pLoadTable;
    bool m_IsIndividual;
};

struct Util
{
    static bool IsDeviceMemory(uptr memory, size_t size)
    {
        const uptr head = nn::os::GetDeviceMemoryAddress();
        const uptr tail = head + nn::os::GetDeviceMemorySize();
        if (head <= memory && (memory + size) <= tail)
        {
            return true;
        }
        return false;
    }
};

/* INTEGRATOR: differs from include/nw/snd/snd_SoundArchiveLoader.h: the target's IndividualWaveInfo is 32 bytes
 * (FILE_ALIGNMENT 32; the copy into the wave buffer is 0x20 bytes, not 0x40). */
class SoundArchiveLoader
{
public:
    static const u32 SIGNATURE_INDIVIDUAL_WAVE = 'C' | ('I' << 8) | ('W' << 16) | ('V' << 24);
    static const u32 FILE_ALIGNMENT = 32;

    struct IndividualWaveInfo
    {
        u32 signature;
        u32 fileId;
        u32 waveIndex;
        u8 padding[SoundArchiveLoader::FILE_ALIGNMENT - sizeof(u32) * 3];

        IndividualWaveInfo(u32 id, u32 index):
            signature(SoundArchiveLoader::SIGNATURE_INDIVIDUAL_WAVE),
            fileId(id),
            waveIndex(index)
        {
        }
    };

    virtual ~SoundArchiveLoader();

private:
    bool LoadIndividualWave(SoundArchive::ItemId warcId, u32 waveIndex,
        SoundMemoryAllocatable* pAllocator, size_t loadBlockSize);
    const void* LoadWaveArchiveTable(SoundArchive::ItemId warcId,
        SoundMemoryAllocatable* pAllocator, size_t loadBlockSize);
    s32 ReadFile(SoundArchive::FileId fileId, void* buffer, size_t size, s32 offset, size_t loadBlockSize);

    virtual const void* SetFileAddressToTable(SoundArchive::FileId fileId, const void* address) = 0;
    virtual const void* GetFileAddressFromTable(SoundArchive::FileId fileId) const = 0;
    virtual const void* GetFileAddressImpl(SoundArchive::FileId fileId) const = 0;

    const SoundArchive* m_pSoundArchive;
};

bool SoundArchiveLoader::LoadIndividualWave(SoundArchive::ItemId warcId, u32 waveIndex,
    SoundMemoryAllocatable* pAllocator, size_t loadBlockSize)
{
    u32 fileId = m_pSoundArchive->GetItemFileId(warcId);
    const void* pWaveArchiveFile = GetFileAddressImpl(fileId);
    if(pWaveArchiveFile == NULL)
    {
        pWaveArchiveFile = LoadWaveArchiveTable(warcId, pAllocator, loadBlockSize);
        if(pWaveArchiveFile == NULL)
        {
            return false;
        }
    }

    WaveArchiveFileReader reader(pWaveArchiveFile, true);

    if(reader.IsLoaded(waveIndex))
    {
        return true;
    }

    const size_t WAVE_FILE_SIZE = reader.GetWaveFileSize(waveIndex);
    const size_t REQUIRED_SIZE = WAVE_FILE_SIZE + sizeof(IndividualWaveInfo);
    void* buffer = pAllocator->Alloc(REQUIRED_SIZE);
    if(buffer == NULL || !Util::IsDeviceMemory(reinterpret_cast<uptr>(buffer), REQUIRED_SIZE))
    {
        return false;
    }

    {
        IndividualWaveInfo iWavInfo(fileId, waveIndex);
        memcpy(buffer, &iWavInfo, sizeof(IndividualWaveInfo));
    }

    {
        void* loadingAddress = static_cast<u8*>(buffer) + sizeof(IndividualWaveInfo);
        s32 readSize = ReadFile(fileId, loadingAddress, WAVE_FILE_SIZE,
            reader.GetWaveFileOffsetFromFileHead(waveIndex), loadBlockSize);
        if(readSize != WAVE_FILE_SIZE)
        {
            return false;
        }
        reader.SetWaveFile(waveIndex, loadingAddress);

        nn::snd::CTR::FlushDataCache(reinterpret_cast<uptr>(loadingAddress), WAVE_FILE_SIZE);
    }
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw
