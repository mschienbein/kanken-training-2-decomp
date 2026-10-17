/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_SoundThread.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SOUND_THREAD_H_
#define NW_SND_SOUND_THREAD_H_

#include "../../nn/os.h"
#include "../os.h"
#include "../ut/ut_LinkList.h"
#include "../ut/ut_Preprocessor.h"
#include "../ut/ut_LinkList.h"
#include "snd_ThreadStack.h"
#include "snd_Util.h"
#include "snd_Global.h"

namespace nw { 
namespace snd { 
namespace internal { 
namespace driver {

class SoundThread
{
public:
    SoundThread();
    
    class SoundFrameCallback
    {
      public:
        ut::LinkListNode m_Link;

        virtual ~SoundFrameCallback() {}
        virtual void OnBeginSoundFrame() {}
        virtual void OnEndSoundFrame() {}
    };

    class PlayerCallback
    {
    public:
        ut::LinkListNode m_Link;

        virtual ~PlayerCallback() {}
        virtual void OnUpdateFrameSoundThread() {}
        virtual void OnUpdateVoiceSoundThread() {}
        virtual void OnShutdownSoundThread() {}
    };

    static SoundThread& GetInstance();
    bool CreateSoundThread(uptr stackBufferAddress, size_t stackBufferSize, s32 priority,
        uptr userThreadStackBufferAddress, size_t userThreadStackBufferSize, s32 userThreadPriority,
        s32 coreNo, bool isEnableGetTick);
    bool PrepareForCreate(bool enableGetTick);

    void Destroy();
    bool IsCreated() const { return m_CreateFlag != 0; }

    void Initialize();
    void Finalize();

    void RegisterSoundFrameCallback(SoundFrameCallback* callback);
    void UnregisterSoundFrameCallback(SoundFrameCallback* callback);

    void RegisterPlayerCallback(PlayerCallback* callback);
    void UnregisterPlayerCallback(PlayerCallback* callback);

    void Pause(bool pauseFlag) { m_PauseFlag = pauseFlag; }
    void FrameProcess();
    void VoiceUpdate();

    void CalcProcessCost(const nn::os::Tick& tick);
    
    void Lock() { m_CriticalSection.Enter(); }
    void Unlock() { m_CriticalSection.Leave(); }

    void SoundThreadProc();
    static void SoundThreadFunc(uptr arg);
    static void UserThreadCallback(uptr arg);

private:
    typedef ut::LinkList< SoundFrameCallback, offsetof(SoundFrameCallback,m_Link)> SoundFrameCallbackList;
    typedef ut::LinkList< PlayerCallback, offsetof(PlayerCallback,m_Link)> PlayerCallbackList;

    nn::os::Tick m_SoundThreadSumTick;
    int m_SoundThreadCount;
    int m_DspCycles;
    Util::PerfHistogram m_PerfHistogram;
    bool m_IsBeginCalcPerf;

    typedef nn::os::InterCoreCriticalSection CriticalSection; // Why typedef this hal

    mutable CriticalSection m_CriticalSection;

    SoundFrameCallbackList  m_SoundFrameCallbackList;
    PlayerCallbackList m_PlayerCallbackList;
    SoundFrameUserCallback  m_UserCallback;
    uptr m_UserCallbackArg;
    s32  m_SoundThreadCoreNo;
    bool m_CreateFlag;
    bool m_PauseFlag;
    bool m_IsEnableGetTick;
    volatile bool m_IsFinalizing;
public:
    struct PerfData
    {
        f32 load;
        u8 voice;
        u8 noteOn;
        u8 padding[2];
    };

    void IncrNoteOnCount() { m_NoteOnCount += 1; }
    int GetPerfDataCount() const { return m_PerfDataCount; }
private:
    PerfData* m_pPerfData;
    int m_NoteOnCount;
    int m_PerfDataNum;
    int m_PerfDataCount;
};

class SoundThreadLock
{
public:
    SoundThreadLock()
    {
        SoundThread::GetInstance().Lock();
    }

    ~SoundThreadLock()
    {
        SoundThread::GetInstance().Unlock();
    }
private:
    NW_DISALLOW_COPY_AND_ASSIGN(SoundThreadLock);
};
} // namespace driver 
} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_SOUND_THREAD_H_

