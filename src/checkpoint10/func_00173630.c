#include "game/notification.h"
extern struct TransformEntry *func_00174454(int group, int index);
struct TransformOwner;
/* The owner is retained for the common transform-operation interface. */
void func_00173630(struct TransformOwner *owner, int index, int group)
{
    struct TransformEntry *entry = func_00174454(group, index);
    entry->x = 0.0f;
    entry->y = 0.0f;
    entry->count = 0;
    entry->flags = 0;
}
