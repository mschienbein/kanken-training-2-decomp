/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_WaveSoundFileReader.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_WAVE_SOUND_FILE_READER_H_
#define NW_SND_WAVE_SOUND_FILE_READER_H_

#include "snd_WaveSoundFile.h"
#include "snd_Global.h"
#include "../ut/ut_BinaryFileFormat.h"

namespace nw {
namespace snd {
namespace internal {

struct WaveSoundInfo;
struct WaveSoundNoteInfo;

class WaveSoundFileReader
{
public:
    static const u32 SIGNATURE_FILE = NW_UT_MAKE_SIGWORD('C', 'W', 'S', 'D'); // CWSD

    WaveSoundFileReader(const void* waveSoundFile);
    bool IsAvailable() const { return m_pHeader != NULL; }

    u32 GetWaveSoundCount() const;
    u32 GetNoteInfoCount(u32 index) const;
    u32 GetTrackInfoCount(u32 index) const;

    bool ReadWaveSoundInfo(WaveSoundInfo* dst, u32 index) const;
    bool ReadNoteInfo(WaveSoundNoteInfo* dst, u32 index, u32 noteIndex) const;

private:
    const WaveSoundFile::FileHeader*    m_pHeader;
    const WaveSoundFile::InfoBlockBody* m_pInfoBlockBody;
};

struct WaveSoundInfo
{
    f32 pitch;
    AdshrCurve adshr;
    u8 pan;
    u8 surroundPan;
    u8 mainSend;
    u8 fxSend[AUX_BUS_NUM];
};

struct WaveSoundNoteInfo
{
    u32 waveArchiveId;
    s32 waveIndex;
    AdshrCurve adshr;
    u8 originalKey;
    u8 pan;
    u8 surroundPan;
    u8 volume;
    f32 pitch;
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_WAVE_SOUND_FILE_READER_H_

