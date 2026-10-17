/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_WaveSoundHandle.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_STREAM_SOUND_HANDLE_H_
#define NW_SND_STREAM_SOUND_HANDLE_H_

#include "../ut/ut_Preprocessor.h"
#include "snd_WaveSound.h"
#include "snd_SoundHandle.h"

namespace nw {
namespace snd {

class WaveSoundHandle
{
public:
    WaveSoundHandle(SoundHandle* pHandle);
    WaveSoundHandle(): 
        m_pSound(NULL) 
    { 
    }

    ~WaveSoundHandle() 
    { 
        DetachSound(); 
    }

    void StartPrepared()
    {
        if (IsAttachedSound())
        {
            m_pSound->StartPrepared();
        }
    }

    void Stop(int fadeFrames)
    {
        if (IsAttachedSound())
        {
            m_pSound->Stop(fadeFrames);
        }
    }

    void Pause(bool flag, int fadeFrames)
    {
        if (IsAttachedSound())
        {
            m_pSound->Pause(flag, fadeFrames);
        }
    }

    bool IsPrepared() const { return IsAttachedSound() && m_pSound->IsPrepared(); }
    bool IsPause() const { return IsAttachedSound() && m_pSound->IsPause(); }

    void FadeIn(int frames)
    {
        if (IsAttachedSound())
        {
            m_pSound->FadeIn(frames);
        }
    }

    void SetVolume(f32 volume, int frames = 0)
    {
        if (IsAttachedSound())
        {
            m_pSound->SetVolume(volume, frames);
        }
    }

    void SetPitch(f32 pitch)
    {
        if (IsAttachedSound())
        { 
            m_pSound->SetPitch(pitch);
        }
    }

    void SetLpfFreq(f32 lpfFreq)
    {
        if (IsAttachedSound())
        { 
            m_pSound->SetLpfFreq(lpfFreq);
        }
    }

    void SetPlayerPriority(int priority)
    {
        if (IsAttachedSound()) 
        {    
            m_pSound->SetPlayerPriority(priority);
        }
    }

    void SetMainSend(f32 send)
    {
        if (IsAttachedSound())
        {
            m_pSound->SetMainSend(send);
        }
    }

    void SetFxSend(AuxBus bus, f32 send)
    {
        if (IsAttachedSound()) 
        {
            m_pSound->SetFxSend(bus, send);
        }
    }

    void detail_AttachSoundAsTempHandle(internal::WaveSound* pSound);

    bool IsAttachedSound() const { return m_pSound != NULL; }
    void DetachSound();
private:
    NW_DISALLOW_COPY_AND_ASSIGN(WaveSoundHandle);

    internal::WaveSound* m_pSound;
};

} // namespace snd
} // namespace nw

#endif

