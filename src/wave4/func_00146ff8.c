struct ArenaList {
    unsigned char reserved00[0x34];
};

struct MemoryArena {
    struct ArenaList first;
    unsigned int first_storage[0x20];
    struct ArenaList second;
    unsigned int second_storage[0x21];
    unsigned int field16c;
    unsigned int field170;
    unsigned int field174;
    unsigned int reserved178;
    void *storage;
    unsigned int word_count;
    unsigned int field184;
    unsigned int field188;
    unsigned char flag18c;
    unsigned char initialized;
};

extern void func_00180884(struct ArenaList *list, unsigned int *storage, unsigned int kind);

void func_00146ff8(struct MemoryArena *arena, void *storage, unsigned int size)
{
    arena->field16c = 0;
    arena->field170 = 0;
    arena->field174 = 0;
    arena->field184 = 0;
    arena->field188 = 0;
    arena->flag18c = 0;
    arena->storage = storage;
    arena->word_count = size >> 2;
    func_00180884(&arena->first, arena->first_storage, 0x20);
    func_00180884(&arena->second, arena->second_storage, 0x21);
    arena->initialized = 1;
}
