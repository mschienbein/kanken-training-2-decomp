#ifndef GAME_AUDIO_TASKS_H
#define GAME_AUDIO_TASKS_H

struct Allocation;
struct ObjectManager;
struct PoolLink;
struct PoolList;
struct StreamEvent;
struct StreamMutex;
struct StreamQueue;
struct TaskPool;
struct WorkerThread;

#include "game/pool_list.h"
#include "game/stream_manager.h"
#include "game/audio_voice.h"
#include "game/object_manager.h"
#include "game/runtime.h"
#include "game/waitable_handle.h"

struct AudioTaskManager {
    struct PoolList lists[3];
    unsigned count;
    unsigned char active;
    unsigned char reserved29[3];
    struct StreamMutex lock;
    struct StreamQueue queue;
    struct StreamEvent *storage[32];
};
struct AudioWorkItem;
struct AudioWorkRequest {
    const void *type;
    struct PoolLink link;
    unsigned signal_state, signal_count;
    unsigned char active;
    unsigned char reserved15[3];
    struct AudioWorkItem *owner, *context;
};
struct AudioWorkItem {
    const void *field00;
    unsigned char reserved04[0x10];
    const void *field14;
    unsigned char reserved18[0x3c];
    unsigned field54;
    unsigned char reserved58[0x0c];
    struct AudioWorkRequest request;
};
struct ObjectPool;
struct SoundSubsystem;
struct GlobalSoundState {
    unsigned char flag00, initialized, flag02, reserved03;
    unsigned capacity;
    struct Allocation work_area;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct GlobalSoundState data_00242fb8;
extern struct SoundSubsystem bss_004b32c0;
extern struct AudioTaskManager *func_00185de4(void);
extern struct StreamQueue *func_00181328(struct StreamQueue *queue);
extern struct WorkerThread *func_00140700(void);
extern struct ObjectPool *func_00185cc0(void);
extern void func_00131398(struct ObjectPool *pool);
extern void func_00131464(struct TaskPool *pool);
extern void func_001313f4(struct SoundSubsystem *subsystem);
extern void func_00131330(struct ObjectManager *manager);
extern void func_001312a4(struct ObjectManager *manager);
extern void func_00131304(struct ObjectManager *manager);
extern void func_00148210(void);
extern void func_001838ec(struct AudioTaskManager *manager);
extern void func_00183f08(struct AudioTaskManager *manager);
/* Cancellation compares the full word with Task::m_Id; it does not
 * dereference a Task or RegistryEntry receiver or narrow to an index. */
extern void func_001d32e0(struct AudioTaskManager *manager, unsigned id);
extern void func_001d32e4(struct AudioTaskManager *manager, unsigned id);
extern void func_001d6b24(unsigned id);
extern void func_001d6678(unsigned id);
extern void func_0017df84(struct AudioTaskManager *manager,
                          struct AudioWorkRequest *request, unsigned category);
#ifdef __cplusplus
}
#endif



#endif
