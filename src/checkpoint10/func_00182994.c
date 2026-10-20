#include "game/object_pool.h"
#include "game/scene_lifecycle.h"
#include "game/region_heap.h"
#include "game/audio_voice.h"

struct RegisteredObject {
    unsigned char reserved[0x88];
    struct RegistryEntry entry;
};
extern struct RegisteredObject *func_0014d784(struct RegisteredObject *object);
extern void func_0017f4f4(struct FreeLink **head, struct FreeLink *node);
void func_00182994(struct ObjectPool *pool, void *object)
{
    struct AudioVoice *child = object;
    struct PoolList *active = &pool->active;
    struct PoolLink *link = &child->link;
    /* ORIGINAL BUG: unlinking precedes the null check, so a null child is unsafe. */
    func_0018d868(active, link);
    if (object) {
        func_0014d784((struct RegisteredObject *)object);
        func_0017f4f4((struct FreeLink **)&pool->free_list, (struct FreeLink *)object);
    }
}
