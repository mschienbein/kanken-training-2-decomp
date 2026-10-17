/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_SoundHeap.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SOUND_HEAP_H_
#define NW_SND_SOUND_HEAP_H_

#include "../../nn/os.h"
#include "snd_FrameHeap.h"
#include "snd_SoundMemoryAllocatable.h"

namespace nw {
namespace snd {

class SoundArchive;
class SoundDataManager;

class SoundHeap : public SoundMemoryAllocatable
{
public:
    typedef void (*DisposeCallback)(void* mem, unsigned long size, void* userArg);

public:
    SoundHeap();
    virtual ~SoundHeap();
    virtual void* Alloc(size_t size);

    void* Alloc(size_t size, DisposeCallback callback, void* callbackArg);
    bool Create(void* startAddress, size_t size);
    void Destroy();
    void Clear();

    bool IsValid() const { return m_FrameHeap.IsValid(); }

    int SaveState();
    void LoadState(int level);

    int GetCurrentLevel() const
    {
        nn::os::CriticalSection::ScopedLock locker(m_CriticalSection);
        return m_FrameHeap.GetCurrentLevel();
    }

    size_t GetSize() const
    {
        nn::os::CriticalSection::ScopedLock locker(m_CriticalSection);
        return m_FrameHeap.GetSize();
    }

    size_t GetFreeSize() const
    {
        nn::os::CriticalSection::ScopedLock locker(m_CriticalSection);
        return m_FrameHeap.GetFreeSize();
    }

    void Dump(nw::snd::SoundDataManager& mgr, nw::snd::SoundArchive& arc) const
    {
        nn::os::CriticalSection::ScopedLock locker(m_CriticalSection);
        m_FrameHeap.Dump(mgr, arc);
    }

private:
    static void DisposeCallbackFunc(void* mem, unsigned long size, void* arg);

    mutable nn::os::CriticalSection m_CriticalSection;
    internal::FrameHeap m_FrameHeap;
};


} // namespace snd
} // namespace nw

#endif // NW_SND_SOUND_HEAP_H_

