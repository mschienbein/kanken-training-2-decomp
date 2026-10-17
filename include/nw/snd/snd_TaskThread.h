/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_TaskThread.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_TASK_THREAD_H_
#define NW_SND_TASK_THREAD_H_

#include "snd_ThreadStack.h"
#include "../../nn/os.h"

namespace nw {
namespace snd {
namespace internal {

class TaskThread
{
public:
    TaskThread();
    ~TaskThread();
    
    bool Create(s32 priority, ThreadStack& stack);
    void Destroy();
    
    bool IsCreated() const { return m_IsCreated != 0; }
    
    void SetPriority(s32 priority) { m_Thread.ChangePriority(priority); }

    bool TryLock() { return m_CriticalSection.TryEnter(); }
    void Lock() { m_CriticalSection.Enter(); }
    void Unlock() { m_CriticalSection.Leave(); }
public:
    static TaskThread& GetInstance();

private:
    void ThreadProc();
    static void ThreadFunc(uptr arg);

    nn::os::Thread m_Thread;
    nn::os::CriticalSection m_CriticalSection;
    volatile bool m_IsFinished;
    bool m_IsCreated;
};

} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_TASK_THREAD_H_

