/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_ExternalSoundPlayer.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_EXTERNAL_SOUND_PLAYER_H_
#define NW_SND_EXTERNAL_SOUND_PLAYER_H_

#include "../ut/ut_LinkList.h"
#include "snd_BasicSound.h"
#include "snd_SoundHandle.h"

namespace nw { 
namespace snd {

class SoundActor;

namespace internal {

class ExternalSoundPlayer
{
public:
    typedef ut::LinkList<BasicSound, offsetof(BasicSound, m_ExtSoundPlayerPlayLink)> SoundList;

public:
    ExternalSoundPlayer();
    ~ExternalSoundPlayer();

    void StopAllSound(int fadeFrames);
    void PauseAllSound(bool flag, int fadeFrames);

    int GetPlayingSoundCount() const { return static_cast<int>(m_SoundList.GetSize()); }
    void SetPlayableSoundCount(int count);
    int GetPlayableSoundCount() const { return m_PlayableCount; }

    bool detail_CanPlaySound(int startPriority);

    bool AppendSound(internal::BasicSound* sound);
    void RemoveSound(internal::BasicSound* sound);

    void Finalize(SoundActor* actor);

    template<class Function>
    Function ForEachSound(Function function, bool reverse = false);

private:
    internal::BasicSound* GetLowestPrioritySound();

    SoundList m_SoundList;
    int m_PlayableCount;
};

template< class Function >
inline Function ExternalSoundPlayer::ForEachSound(Function function, bool reverse)
{
    if (reverse)
    {
        for (SoundList::ReverseIterator itr = m_SoundList.GetBeginReverseIter(); itr != m_SoundList.GetEndReverseIter(); )
        {
            SoundList::ReverseIterator curItr = itr;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle(&(*curItr));
            function(handle);
            if (handle.IsAttachedSound()) 
            {
                itr++;
            }
        }
    }
    else
    {
        for (SoundList::Iterator itr = m_SoundList.GetBeginIter(); itr != m_SoundList.GetEndIter(); )
        {
            SoundList::Iterator curItr = itr++;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle(&(*curItr));
            function(handle);
        }
    }
    return function;
}

} // namespace nw
} // namespace snd
} // namespace internal

#endif // NW_SND_EXTERNAL_SOUND_PLAYER_H_

