/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_VoiceManager.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * Modified 2026-10-20: target Voice link and one shared native manager storage.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_VOICE_MANAGER_H
#define NW_SND_VOICE_MANAGER_H

#include "snd_Voice.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {

class VoiceManager
{
    friend class Voice;
    
public:
    typedef ut::LinkList<Voice, offsetof(Voice, link)> VoiceList;

    static VoiceManager& GetInstance();

    size_t GetRequiredMemSize(int voiceCount);
    void Initialize(void* mem, size_t memSize);
    void Finalize();

    Voice* AllocVoice(int voiceChannelCount, int priority, Voice::VoiceCallback callback, void* callbackData);
    void FreeVoice(Voice* voice);

    void StopAllVoices();
    
    void UpdateAllVoices();
    void UpdateAllVoicesSync(u32 syncFlag);
    
    int GetVoiceCount() const;
    unsigned long GetActiveCount() const;
    unsigned long GetFreeCount() const;
    const VoiceList& GetVoiceList() const;
private:
    VoiceManager();
    ~VoiceManager();

    void ChangeVoicePriority(Voice* voice);
    int DropLowestPriorityVoice(int priority);
    void UpdateEachVoicePriority(const VoiceList::Iterator& beginItr,const VoiceList::Iterator& endItr);

    void AppendVoiceList(Voice* voice);
    void RemoveVoiceList(Voice* voice);

public:
    bool m_Initialized;
    VoiceList m_PrioVoiceList;
    VoiceList m_FreeVoiceList;
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_VOICE_MANAGER_H

