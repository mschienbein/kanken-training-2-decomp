struct StorageSpan {
    void *buffer;
    unsigned int capacity;
};

struct BufferedStorage {
    unsigned int state[10];
    unsigned char active;
    unsigned char reserved29[3];
    struct StorageSpan span;
};

struct BufferedStorage *func_00112b80(struct BufferedStorage *storage,
                                     void *buffer, unsigned int capacity)
{
    storage->state[0] = 0;
    storage->state[1] = 0;
    storage->state[2] = 0;
    storage->state[3] = 0;
    storage->state[4] = 0;
    storage->state[5] = 0;
    storage->state[6] = 0;
    storage->state[7] = 0;
    storage->state[8] = 0;
    storage->state[9] = 0;
    storage->active = 0;
    {
        struct StorageSpan span;
        span.buffer = buffer;
        span.capacity = capacity;
        storage->span = span;
    }
    return storage;
}
