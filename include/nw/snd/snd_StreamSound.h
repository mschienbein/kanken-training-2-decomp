/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_StreamSound.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_STREAM_SOUND_H_
#define NW_SND_STREAM_SOUND_H_

#include "snd_SoundInstanceManager.h"
#include "snd_BasicSound.h"
#include "snd_StreamSoundPlayer.h"
#include "snd_StreamBufferPool.h"

namespace nw { 
namespace snd { 

class StreamSoundHandle;

namespace internal {

class StreamSound;

typedef SoundInstanceManager<StreamSound, driver::StreamSoundPlayer> StreamSoundInstanceManager;

class StreamSound : public BasicSound
{
    friend class StreamSoundHandle;
public:
    NW_UT_RUNTIME_TYPEINFO;

    static const int FILE_STREAM_BUFFER_SIZE = 512;

    StreamSound(StreamSoundInstanceManager& manager);

    void Setup(driver::StreamBufferPool* pBufferPool, int allocChannelCount, u16 allocTrackFlag);
    void Prepare(driver::StreamSoundPlayer::StartOffsetType startOffsetType, s32 offset, io::FileStream* pFileStream);

    virtual void Initialize();
    virtual void Finalize();
    virtual bool IsPrepared() const;
    virtual bool IsAttachedTempSpecialHandle();
    virtual void DetachTempSpecialHandle();
    virtual void UpdateMoveValue();
    virtual void UpdateParam();
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() { return &m_StreamSoundPlayerInstance; }
    virtual void OnUpdatePlayerPriority();

    bool IsSuspendByLoadingDelay() const;

    void* GetFileStreamBuffer() { return m_FileStreamBuffer; }
    long GetFileStreamBufferSize() { return FILE_STREAM_BUFFER_SIZE; }

    void SetTrackVolume(unsigned long trackBitFlag, float volume, int frames = 0);
    void SetTrackPan(unsigned long trackBitFlag, float pan);
    void SetTrackSurroundPan( unsigned long trackBitFlag, float span );

    bool ReadStreamDataInfo(StreamDataInfo* info) const;
    long GetPlayLoopCount() const;
    long GetPlaySamplePosition() const;
    float GetFilledBufferPercentage() const;

    const driver::StreamSoundPlayer& detail_GetStreamSoundPlayer() const { return m_StreamSoundPlayerInstance; }
    driver::StreamSoundPlayer& detail_GetStreamSoundPlayer() { return m_StreamSoundPlayerInstance; }
private:
    driver::StreamSoundPlayer m_StreamSoundPlayerInstance;
public:
    StreamSoundHandle* m_pTempSpecialHandle;
private:
    StreamSoundInstanceManager& m_Manager;

    MoveValue<float, int> m_TrackVolume[driver::StreamSoundPlayer::STRM_TRACK_NUM];

    io::FileStream* m_pFileStream;
    int m_FileStreamBuffer[FILE_STREAM_BUFFER_SIZE / sizeof(int)];
    u16 m_AllocTrackFlag;
    bool m_InitializeFlag;
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_STREAM_SOUND_H_

