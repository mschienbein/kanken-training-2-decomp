/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_WaveArchiveFileReader.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_WAVE_ARCHIVE_FILE_READER_H_
#define NW_SND_WAVE_ARCHIVE_FILE_READER_H_

#include "snd_WaveArchiveFile.h"
#include "../ut/ut_BinaryFileFormat.h"

namespace nw { 
namespace snd { 
namespace internal {

class WaveArchiveFileReader
{
public:
    static const u32 SIGNATURE_FILE = NW_UT_MAKE_SIGWORD('C', 'W', 'A', 'R');
    static const u32 SIGNATURE_WARC_TABLE;

    WaveArchiveFileReader(const void* pWaveArchiveFile, bool isIndividual = false);

    void InitializeFileTable();
    bool IsAvailable() const { return m_pHeader != NULL; }

    u32 GetWaveFileCount() const;
    u32 GetWaveFileSize(u32 waveIndex) const;
    u32 GetWaveFileOffsetFromFileHead(u32 waveIndex) const;

    const void* GetWaveFile(u32 waveIndex) const;

    const void* SetWaveFile(u32 waveIndex, const void* pWaveFile);
    bool IsLoaded(u32 waveIndex) const
    {
        if (GetWaveFile(waveIndex) != NULL)
        {
            return true;
        }
        return false;
    }

    bool HasIndividualLoadTable() const;

private:
    struct IndividualLoadTable
    {
        const void* waveFile[1];
    };


    const void* GetWaveFileForWhole(u32 waveIndex) const
    {
        u32 offset = m_pInfoBlockBody->GetOffsetFromFileBlockBody(waveIndex);
        return ut::AddOffsetToPtr(&m_pHeader->GetFileBlock()->body, offset);
    }
    
    const void* GetWaveFileForIndividual(u32 waveIndex) const
    {
        return m_pLoadTable->waveFile[ waveIndex ];
    }

    const WaveArchiveFile::FileHeader*      m_pHeader;
    const WaveArchiveFile::InfoBlockBody*   m_pInfoBlockBody;
    IndividualLoadTable*                    m_pLoadTable;
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_WAVE_ARCHIVE_FILE_READER_H_

