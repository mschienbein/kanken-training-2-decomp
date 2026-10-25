/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * snd_SoundArchiveLoader.cpp. The image's 16-byte individual-load reader
 * and 32-byte IndividualWaveInfo are now declared in the canonical headers.
 * [INFERENCE] Native method spelling; the GetFileAddressImpl slot follows
 * the retained original call. No parallel archive/reader classes are kept. */
#include <preinclude.h>
#include <nw/snd/snd_SoundArchiveLoader.h>
#include <nw/snd/snd_SoundMemoryAllocatable.h>
#include <nw/snd/snd_WaveArchiveFileReader.h>
#include <nn/snd.h>
#include <string.h>

namespace nw { namespace snd { namespace internal {
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

} } }
