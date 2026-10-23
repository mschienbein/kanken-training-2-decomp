#include <stddef.h>
#include "game/audio_tasks.h"

class TaskQueueLock {
    StreamMutex &mutex;
public:
    explicit TaskQueueLock(StreamMutex &value) : mutex(value)
    {
        func_00192bd8(&mutex);
    }
    ~TaskQueueLock()
    {
        func_00195d74(&mutex);
    }
};

extern "C" AudioWorkRequest *func_0017f504(AudioTaskManager *manager,
                                         int priority, bool remove)
{
    TaskQueueLock lock(manager->lock);
    if (manager->lists[priority].count == 0)
        return 0;
    PoolLink *first = manager->lists[priority].sentinel.next;
    AudioWorkRequest *task = reinterpret_cast<AudioWorkRequest *>(
        reinterpret_cast<char *>(first) - offsetof(AudioWorkRequest, link));
    if (remove)
        func_00191178(&manager->lists[priority], first);
    return task;
}
