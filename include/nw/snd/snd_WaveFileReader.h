/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_WaveFileReader.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_WAVE_FILE_READER_H_
#define NW_SND_WAVE_FILE_READER_H_

#include "snd_WaveFile.h"

namespace nw {
namespace snd {
namespace internal {

class WaveFileReader
{
public:
    static const u32 SIGNATURE_FILE = NW_UT_MAKE_SIGWORD('C', 'W', 'A', 'V');
    static SampleFormat GetSampleFormat(u8 encodeMethod);

    WaveFileReader(const void* waveFile);

    bool ReadWaveInfo(WaveInfo* info, const void* waveDataOffsetOrigin = NULL) const;
private:
    const void* GetWaveDataAddress(const WaveFile::ChannelInfo* info, const void* waveDataOffsetOrigin ) const;

    const WaveFile::InfoBlockBody*  m_pInfoBlockBody;
    const void*                     m_pDataBlockBody;
};

} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_WAVE_FILE_READER_H_

