/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_WaveSound.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_WAVE_SOUND_H_
#define NW_SND_WAVE_SOUND_H_

#include "snd_SoundInstanceManager.h"
#include "snd_BasicSound.h"
#include "snd_WaveSoundPlayer.h"
#include "snd_Task.h"
#include "snd_PlayerHeapDataManager.h"

namespace nw { 
namespace snd {

class WaveSoundHandle;
class SoundDataManager;

namespace internal {

class WaveSound;

typedef SoundInstanceManager<WaveSound, driver::WaveSoundPlayer> WaveSoundInstanceManager;

class WaveSound : public BasicSound
{
    friend class WaveSoundHandle;

public:
    NW_UT_RUNTIME_TYPEINFO;

    WaveSound(WaveSoundInstanceManager& manager);
    virtual ~WaveSound() {}
    virtual void Initialize();
    virtual void Finalize();
    virtual bool IsPrepared() const { return m_PreparedFlag; }
    virtual bool IsAttachedTempSpecialHandle(){ return m_pTempSpecialHandle != NULL; }
    virtual void DetachTempSpecialHandle(){}

    struct StartInfo
    {
        s32 index;
        driver::WaveSoundPlayer::StartOffsetType startOffsetType;
        s32 startOffset;
        const driver::WaveSoundPlayer::WaveSoundCallback* callback;
        u32 callbackData;
    };

    struct LoadInfo
    {
        const SoundArchive* arc;
        const SoundDataManager* mgr;
        const LoadItemInfo* wsd;
    };

    void Prepare(const void* wsdFile, const StartInfo& startInfo);
    bool RegisterDataLoadTask(const LoadInfo& loadInfo, const StartInfo& startInfo);
    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool fix);
    long GetPlaySamplePosition() const;

    static void NotifyAsyncLoadFinishedFunc(bool result, const LoadItemInfo* wsd, void* userData);
protected:
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle(){ &m_WaveSoundPlayerInstance; }
    virtual void OnUpdatePlayerPriority() 
    {
        int priority = CalcCurrentPlayerPriority();
        m_pManager.UpdatePriority(this, priority);
    }

    typedef void (*NotifyAsyncLoadFinished)(bool result, const LoadItemInfo* wsd, void* userData);

    class DataLoadTask : public Task
    {
    public:
        DataLoadTask();
        virtual void Execute();

        PlayerHeapDataManager* m_pDataManager;
        PlayerHeap* m_pAllocator;
        NotifyAsyncLoadFinished m_Callback;
        void* m_CallbackData;

        const SoundDataManager* m_pSoundDataManager;
        const SoundArchive* m_pSoundArchive;
        LoadItemInfo m_LoadInfoWsd;

        s32 m_Index;
    };
public:

    driver::WaveSoundPlayer m_WaveSoundPlayerInstance;
    WaveSoundHandle* m_pTempSpecialHandle;
    WaveSoundInstanceManager& m_pManager;

    DataLoadTask m_DataLoadTask;
    PlayerHeapDataManager m_DataManager;
    StartInfo m_StartInfo;

    volatile bool m_LoadingFlag;
    bool m_PreparedFlag;
    bool m_InitializeFlag;
};

} // namespace internal
} // namespace snd
} // namespace mw

#endif // NW_SND_WAVE_SOUND_H_

