#include "game/command_group.h"

struct TaskQueueNode {
    unsigned char reserved00[0x18];
    unsigned char kind;
    unsigned char reserved19[3];
    struct PoolLink link;
};
struct TaskQueueContext {
    unsigned char reserved00[0x18];
    struct PoolList special;
    struct PoolList normal;
};

static struct PoolLink *list_end(struct PoolList *list)
{
    return &list->sentinel;
}

struct PoolLink *func_0016a0d0(struct TaskQueueContext *context,
                              struct TaskQueueNode *node)
{
    if (node->kind == 2) {
        struct PoolList *list = &context->special;
        return func_00191154(list, list_end(list), &node->link);
    } else {
        struct PoolList *list = &context->normal;
        return func_00191154(list, list_end(list), &node->link);
    }
}
