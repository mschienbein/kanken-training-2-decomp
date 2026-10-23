#include "game/audio_tasks.h"
#include "game/service_result.h"

/* Private interface witness only: this function remains c012-owned. */
namespace {
struct TaskIterator {
    PoolLink *node;
    explicit TaskIterator(PoolLink *value) : node(value) {}
};
static TaskIterator insert_link(PoolList &list, TaskIterator position, PoolLink &node)
{
    return TaskIterator(func_00191154(&list, position.node, &node));
}
static TaskIterator erase_link(PoolList &list, TaskIterator position)
{
    return TaskIterator(func_0018d868(&list, position.node));
}
static void release_group(TaskPool *pool, AudioVoiceGroup *group)
{
    erase_link(pool->active, TaskIterator(&group->link));
    insert_link(pool->available, TaskIterator(&pool->available.sentinel), group->link);
}
}


void func_0017e764(TaskPool *pool, AudioVoiceGroup *group)
{
    release_group(pool, group);
}

