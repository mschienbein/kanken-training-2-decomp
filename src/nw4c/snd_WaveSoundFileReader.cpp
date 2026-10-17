/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_WaveSoundFileReader.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_WaveSoundFileReader.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_WaveSoundFileReader.h>
#include <nw/snd/snd_WaveFile.h>

namespace nw {
namespace snd {
namespace internal {
namespace
{

const u32 SUPPORTED_FILE_VERSION = 0x01000000;
const u32 CURRENT_FILE_VERSION = 0x01000100;

const u32 SIGNATURE_INFO_BLOCK  = NW_UT_MAKE_SIGWORD('I', 'N', 'F', 'O');

} // namespace

bool WaveSoundFileReader::ReadNoteInfo(WaveSoundNoteInfo* dst, u32 index, u32 noteIndex) const
{
    const WaveSoundFile::NoteInfo& src = m_pInfoBlockBody->GetWaveSoundData(index).GetNoteInfo(noteIndex);
    const Util::WaveId* pWaveId = m_pInfoBlockBody->GetWaveIdTable().GetWaveId(src.waveIdTableIndex);

    if (pWaveId == NULL)
    {
        return false;
    }
    
    dst->waveArchiveId = pWaveId->waveArchiveId;
    dst->waveIndex = pWaveId->waveIndex;
    dst->pitch = src.GetPitch();
    dst->adshr = src.GetAdshrCurve();
    dst->originalKey = src.GetOriginalKey();
    dst->pan = src.GetPan();
    dst->surroundPan = src.GetSurroundPan();
    dst->volume = src.GetVolume();
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw

