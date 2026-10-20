#include "game/audio_voice.h"
#include "game/functions.h"

struct AudioChannel;

struct TaskPool {
    unsigned int reserved00;
    struct PoolList active;
};

#include <stddef.h>

extern void func_001835b8(struct AudioVoiceGroup *group);
extern void func_00183550(struct AudioVoiceGroup *group);

int func_0015477c(struct TaskPool *pool, int priority)
{
    struct AudioVoiceGroup *group;
    int released;
    void (*callback)(struct AudioVoiceGroup *, int, void *);
    void *context;
    if (!pool->active.count)
        return 0;
    group = (struct AudioVoiceGroup *)((unsigned char *)pool->active.sentinel.next
                                      - offsetof(struct AudioVoiceGroup, link));
    if (group->priority > priority)
        return 0;
    released = group->count;
    callback = group->callback;
    context = group->context;
    func_001835b8(group);
    func_00183550(group);
    if (callback)
        callback(group, 2, context);
    return released;
}
