/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_PlayerHeap.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_PLAYER_HEAP_H_
#define NW_SND_PLAYER_HEAP_H_

#include "snd_SoundMemoryAllocatable.h"
#include "../ut/ut_LinkList.h"

namespace nw {
namespace snd {

class SoundPlayer;

namespace internal {

class BasicSound;

class PlayerHeap : public SoundMemoryAllocatable
{
public:
    PlayerHeap();
    virtual ~PlayerHeap();
    virtual void* Alloc(size_t size);

    void AttachSoundPlayer(SoundPlayer* player) { m_pPlayer = player; }

    void AttachSound(BasicSound* sound);
    void DetachSound(BasicSound* sound);

    bool Create(void* startAddress, size_t size);
    void Destroy();

    void Clear();

    bool IsValid() const { return m_pAllocAddress != NULL; }

    size_t GetFreeSize() const;

private:
    BasicSound* m_pSound;
    SoundPlayer* m_pPlayer;

    void* m_pStartAddress;
    void* m_pEndAddress;
    void* m_pAllocAddress;

public:
    ut::LinkListNode m_Link;
};


} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_PLAYER_HEAP_H_

