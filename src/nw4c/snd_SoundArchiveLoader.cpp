/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_SoundArchiveLoader.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
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

SoundArchiveLoader::SoundArchiveLoader(): 
    m_pSoundArchive(NULL),
    m_LoadDataCallCount(0),
    m_IsCancelLoading(false)
{
}


const void* SoundArchiveLoader::GetFileAddressFromSoundArchive(SoundArchive::FileId fileId) const
{
    if(m_pSoundArchive == NULL)
    {
        return NULL;
    }
    return m_pSoundArchive->detail_GetFileAddress(fileId);
}

} // namespace internal
} // namespace snd
} // namespace nw

