#include "game/graphics_resources.h"

void func_0017f3d8(struct GraphicsResourceLink *entry, unsigned key)
{
    struct GraphicsResourceTable *table = data_00242a0c;
    unsigned bucket = key & 0x1ff;
    struct GraphicsResourceLink *previous = table->buckets[bucket];
    struct GraphicsResourceLink *next;
    if (previous == 0) {
        table->buckets[bucket] = entry;
    } else if (key < previous->identifier) {
        entry->next = previous;
        table->buckets[bucket] = entry;
    } else {
        for (next = previous->next; next != 0; next = next->next) {
            if (key < next->identifier) {
                previous->next = entry;
                entry->next = next;
                break;
            }
            previous = next;
        }
        if (next == 0)
            previous->next = entry;
    }
}
