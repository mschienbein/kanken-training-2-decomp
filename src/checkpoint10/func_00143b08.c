#include <stddef.h>
#include "game/audio_tasks.h"

extern struct StreamQueue *func_00181328(struct StreamQueue *queue);

struct AudioTaskManager *func_00143b08(struct AudioTaskManager *manager)
{
    struct StreamQueue *queue = &manager->queue;
    struct StreamQueue *completed = queue;
    func_00183f18(queue);
    /* Both callees return their member pointer (ARM C++ constructor-return shape);
     * the second result is converted back to the containing manager. */
    return (struct AudioTaskManager *)((char *)func_00181328(completed)
           - offsetof(struct AudioTaskManager, queue));
}
