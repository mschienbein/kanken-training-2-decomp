/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_StreamSoundFileReader.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_STREAM_SOUND_FILE_READER_H_
#define NW_SND_STREAM_SOUND_FILE_READER_H_

#include "snd_StreamSoundFile.h"
#include "snd_Global.h"

namespace nw {
namespace snd {
namespace internal {

class StreamSoundFileReader
{
public:
    struct TrackInfo
    {
        u8 volume;
        u8 pan;
        u8 channelCount;
        u8 globalChannelIndex[WAVE_CHANNEL_MAX];
    };

    StreamSoundFileReader();
    void Initialize(const void* streamSoundFile);
    void Finalize();
    bool IsAvailable()const { return m_pHeader != NULL; }

    bool IsValidFileHeader(const void* streamSoundFile) const;

    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* strmInfo ) const;
    bool ReadStreamTrackInfo(TrackInfo* trackInfo, int trackIndex ) const;
    bool ReadDspAdpcmChannelInfo(DspAdpcmParam* param, DspAdpcmLoopParam* loopParam, int channelIndex) const;

    u32 GetChannelCount() const { return m_pInfoBlockBody->GetChannelInfoTable()->GetChannelCount(); }

    u32 GetTrackCount() const { return m_pInfoBlockBody->GetTrackInfoTable()->GetTrackCount(); }

    u32 GetSeekBlockOffset() const
    {
        if (IsAvailable())
            return m_pHeader->GetSeekBlockOffset();
        return 0;
    }

    u32 GetDataBlockOffset() const
    {
        if (IsAvailable())
            return m_pHeader->GetDataBlockOffset();
        return 0;
    }
private:
    const StreamSoundFile::FileHeader*      m_pHeader;
    const StreamSoundFile::InfoBlockBody*   m_pInfoBlockBody;
};

} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_STREAM_SOUND_FILE_READER_H_

