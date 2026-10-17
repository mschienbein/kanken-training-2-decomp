/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_TaskManager.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_TASK_MANAGER_H_
#define NW_SND_TASK_MANAGER_H_

#include "snd_Task.h"
#include "snd_Util.h"
#include "../ut/ut_LinkList.h"
#include "../../nn/os.h"

namespace nw {
namespace snd {
namespace internal {

class TaskManager
{
public:
    enum TaskPriority
    {
        PRIORITY_LOW,
        PRIORITY_MIDDLE,
        PRIORITY_HIGH
    };
    
    static TaskManager& GetInstance();
    
    void Initialize();
    void Finalize();

    void ExecuteTask();
    void AppendTask(Task* task, TaskPriority priority);
    void CancelTask(Task* task);
    void CancelTaskById(u32 id);
    void CancelAllTask();

    void WaitTask();
    void CancelWaitTask();
    
private:
    static const u32 PRIORITY_NUM = PRIORITY_HIGH - PRIORITY_LOW + 1;
    static const u32 THREAD_MESSAGE_BUFSIZE = 32;
    
    enum Message
    {
        MESSAGE_APPEND
    };
    
    typedef ut::LinkList< Task, offsetof(Task,m_TaskLink)> TaskList;
    
    TaskManager();
    Task* PopTask();
    bool RemoveTask(Task* task);
    void RemoveTaskById(u32 id);
    
    Task* GetNextTask();
    Task* GetNextTask( TaskPriority priority, bool doRemove );
    
    TaskList m_TaskList[PRIORITY_NUM];
    Task* volatile m_pCurrentTask;
    volatile bool m_IsWaitTaskCancel;
    nn::os::CriticalSection m_CriticalSection;
    nn::os::BlockingQueue   m_BlockingQueue;
    uptr                    m_MsgBuffer[THREAD_MESSAGE_BUFSIZE];
};


} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_TASK_MANAGER_H_

