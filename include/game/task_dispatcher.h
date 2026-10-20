#ifndef GAME_TASK_DISPATCHER_H
#define GAME_TASK_DISPATCHER_H

#include <stddef.h>
#include "game/synchronization.h"
#include "game/audio_tasks.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


extern "C" {
extern void func_0014b7a0(struct AudioTaskManager *dispatcher);
extern void func_0014b6f8(struct AudioTaskManager *dispatcher);
extern struct StreamManager *func_00185bfc(void);
extern void func_00140904(struct StreamManager *manager);
extern struct AudioWorkRequest *func_0017f504(struct AudioTaskManager *dispatcher,
                                            int priority, int remove);
extern struct StreamEvent *func_001829e0(struct StreamQueue *queue);
}
#endif

#endif
