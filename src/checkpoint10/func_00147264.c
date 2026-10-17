/* FreeListPool matches src/sdk2/func_00180768.c. */
struct FreeListNode { struct FreeListNode *next; };
struct FreeListPool { struct FreeListNode *head; };
struct BlockPool {
    struct FreeListPool free_list;
    unsigned char reserved04[12];
    unsigned char initialized;
    unsigned char reserved11[3];
    unsigned count;
    void *storage;
    unsigned size;
};
extern unsigned func_00180768(struct FreeListPool *pool, void *storage,
                             unsigned size, unsigned element_size);

struct PoolStorage { void *memory; unsigned size; };
static unsigned populate_pool(struct FreeListPool *pool, struct PoolStorage storage)
{
    return func_00180768(pool, storage.memory, storage.size, 0x1b8);
}
void func_00147264(struct BlockPool *pool, void *storage, unsigned size)
{
    struct PoolStorage backing;
    backing.memory = storage;
    backing.size = size;
    if (!pool->initialized) {
        unsigned count = populate_pool(&pool->free_list, backing);
        pool->storage = backing.memory;
        pool->count = count;
        pool->initialized = 1;
        pool->size = backing.size;
    }
}
