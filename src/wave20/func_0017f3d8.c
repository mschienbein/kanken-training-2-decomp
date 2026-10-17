struct HashEntry {
    unsigned reserved00[2];
    unsigned key;
    struct HashEntry *next;
};
struct HashTable {
    unsigned reserved00[4];
    struct HashEntry *buckets[512];
};
extern struct HashTable *data_00242a0c;

void func_0017f3d8(struct HashEntry *entry, unsigned key)
{
    struct HashTable *table = data_00242a0c;
    unsigned bucket = key & 0x1ff;
    struct HashEntry *previous = table->buckets[bucket];
    struct HashEntry *next;
    if (previous == 0) {
        table->buckets[bucket] = entry;
    } else if (key < previous->key) {
        entry->next = previous;
        table->buckets[bucket] = entry;
    } else {
        for (next = previous->next; next != 0; next = next->next) {
            if (key < next->key) {
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
