/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_SoundPlayer.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SOUND_PLAYER_H_
#define NW_SND_SOUND_PLAYER_H_

#include "../ut/ut_LinkList.h"
#include "snd_BasicSound.h"
#include "snd_PlayerHeap.h"

namespace nw {
namespace snd {

class SoundPlayer
{
public:
    typedef ut::LinkList<internal::PlayerHeap, offsetof(internal::PlayerHeap,m_Link) > PlayerHeapList;
    typedef ut::LinkList<internal::BasicSound, offsetof(internal::BasicSound,m_SoundPlayerPlayLink) > SoundList;
    typedef ut::LinkList<internal::BasicSound, offsetof(internal::BasicSound,m_SoundPlayerPriorityLink) > PriorityList;

public:
    SoundPlayer();
    ~SoundPlayer();

    void Update();

    void StopAllSound(int fadeFrames);
    void PauseAllSound(bool flag, int fadeFrames);

    void SetVolume(float volume);
    float GetVolume() const { return m_Volume; }

    void SetLpfFreq(float lpfFreq);
    float GetLpfFreq() const { return m_LpfFreq; }

    void SetBiquadFilter(int type, float value);
    int GetBiquadFilterType() const { return m_BiquadType; }
    float GetBiquadFilterValue() const { return m_BiquadValue; }

    void SetMainSend(float send);
    float GetMainSend() const { return m_MainSend; }

    void SetFxSend(AuxBus bus, float send);
    float GetFxSend(AuxBus bus) const { return m_FxSend[bus]; }

    void SetPlayableSoundCount(int count);
    int GetPlayingSoundCount() const { return static_cast<int>(m_SoundList.GetSize()); }

    int GetPlayableSoundCount() const { return m_PlayableCount; }

    void detail_SetPlayableSoundLimit(int limit);
    bool detail_CanPlaySound(int startPriority);

    bool detail_AppendSound(internal::BasicSound* pSound);
    void detail_RemoveSound(internal::BasicSound* pSound);

    void detail_SortPriorityList();
    void detail_SortPriorityList(internal::BasicSound* pSound);

    void detail_AppendPlayerHeap(internal::PlayerHeap* pHeap);
    void detail_RemovePlayerHeap(internal::PlayerHeap* pHeap);

    internal::PlayerHeap* detail_AllocPlayerHeap(internal::BasicSound* pSound);
    void detail_FreePlayerHeap(internal::BasicSound* pSound);

private:
    internal::BasicSound* GetLowestPrioritySound() { return &m_PriorityList.GetFront(); }
    void InsertPriorityList(internal::BasicSound* pSound);
    void RemovePriorityList(internal::BasicSound* pSound);
    void RemoveSoundList(internal::BasicSound* pSound);

    SoundList       m_SoundList;
    PriorityList    m_PriorityList;
    PlayerHeapList  m_PlayerHeapList;

    int m_PlayableCount;
    int m_PlayableLimit;

    float m_Volume;
    float m_LpfFreq;
    int m_BiquadType;
    float m_BiquadValue;
    float m_MainSend;
    float m_FxSend[AUX_BUS_NUM];
};

} // namespace snd
} // namespace nw


#endif // NW_SND_SOUND_PLAYER_H_

