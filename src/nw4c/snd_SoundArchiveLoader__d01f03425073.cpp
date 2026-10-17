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

bool SoundArchiveLoader::detail_LoadWaveArchiveByWaveSoundFile(const void* wsdFile, s32 wsdIndex, 
    SoundMemoryAllocatable* pAllocator)
{
    if(wsdFile == NULL)
    {
        return false;
    }

    u32 warcId = SoundArchive::INVALID_ID;
    u32 waveIndex;
    {
        internal::WaveSoundFileReader reader(wsdFile);
        internal::WaveSoundNoteInfo info;
        if(!reader.ReadNoteInfo(&info, wsdIndex, 0))
        {
            return false;
        }
        warcId = info.waveArchiveId;
        waveIndex = info.waveIndex;
    }
    if(!LoadWaveArchiveImpl(warcId, waveIndex, pAllocator, LOAD_WARC))
    {
        return false;
    }
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw

