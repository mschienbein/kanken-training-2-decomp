/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_StreamSoundFileLoader.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_STREAM_SOUND_FILE_LOADER_H_
#define NW_SND_STREAM_SOUND_FILE_LOADER_H_

#include "../io/io_FileStream.h"
#include "snd_StreamSoundFileReader.h"

namespace nw {
namespace snd {
namespace internal {

class StreamSoundFileLoader
{
public:
    StreamSoundFileLoader(io::FileStream& stream): 
        m_Stream(stream)
    {
    }

    bool LoadFileHeader(void* buffer, unsigned long size);

    bool ReadStreamInfo( StreamSoundFile::StreamSoundInfo* strmInfo) const;
    bool ReadStreamTrackInfo(
        StreamSoundFileReader::TrackInfo* trackInfo, int trackIndex) const;
    bool ReadDspAdpcmChannelInfo(DspAdpcmParam* adpcmParam,
        DspAdpcmLoopParam* adpcmLoopParam, int channelIndex) const;
    u32 GetChannelCount() const;
    u32 GetTrackCount() const;

    u32 GetDataBlockOffset() const
    {
        return m_Reader.GetDataBlockOffset();
    }

    bool ReadAdpcBlockData(u16* yn1, u16* yn2, int blockIndex, int channelCount);

private:
    io::FileStream& m_Stream;
    StreamSoundFileReader m_Reader;
};

} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_STREAM_SOUND_FILE_LOADER_H_

