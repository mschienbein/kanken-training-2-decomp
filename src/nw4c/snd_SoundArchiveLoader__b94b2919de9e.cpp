/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_SoundArchiveLoader.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_SoundArchiveLoader.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_SoundArchiveLoader.h>
#include <nw/snd/snd_SoundMemoryAllocatable.h>
#include <nw/snd/snd_BankFileReader.h>
#include <nw/snd/snd_GroupFileReader.h>
#include <nw/snd/snd_WaveSoundFileReader.h>
#include <nw/snd/snd_WaveArchiveFileReader.h>

#include <nw/io/io_FileStream.h>

namespace nw {
namespace snd {
namespace internal {

namespace
{

const u32 REQUIRED_SIZE_FOR_LOAD_WARC_FILE_HEADER =
    NW_ROUND_UP_32B(sizeof(internal::WaveArchiveFile::FileHeader) + 32);

class FileStreamHandle
{
public:

private:
    io::FileStream* m_pStream;
};

}

bool SoundArchiveLoader::LoadWaveArchiveImpl(SoundArchive::ItemId warcId, u32 waveIndex, 
    SoundMemoryAllocatable* pAllocator, u32 loadFlag, size_t loadBlockSize)
{
    SoundArchive::WaveArchiveInfo info;
    if(!m_pSoundArchive->ReadWaveArchiveInfo(warcId, &info))
    {
        return false;
    }
    if(info.isLoadIndividual)
    {
        if(!LoadIndividualWave(warcId, waveIndex, pAllocator, loadBlockSize))
        {
            return false;
        }
    }
    else
    {
        if(!LoadWaveArchive(warcId, pAllocator, loadFlag, loadBlockSize))
        {
            return false;
        }
    }
    return true;
}

inline bool SoundArchiveLoader::LoadWaveArchive(SoundArchive::ItemId warcId, SoundMemoryAllocatable* pAllocator, 
    u32 loadFlag, size_t loadBlockSize)
{
    if(loadFlag & LOAD_WARC)
    {
        u32 fileId = m_pSoundArchive->GetItemFileId(warcId);
        const void* pFile = LoadImpl(fileId, pAllocator, loadBlockSize, true);
        if(pFile == NULL)
        {
            return false;
        }
    }
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw

