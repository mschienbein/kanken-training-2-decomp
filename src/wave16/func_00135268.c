#include "game/resource_runtime.h"

struct RenderEntryState {
    float position[3];
    float angles[3];
    float scale[3];
    float parameters[5];
    unsigned value38;
    unsigned value3c;
    unsigned value40;
    void *link44;
    unsigned value48;
    unsigned char reserved4c[0x40];
    unsigned char type8c;
    unsigned char enabled8d;
    unsigned char flag8e;
    unsigned char mode8f;
    unsigned char flag90;
    unsigned char flag91;
    unsigned char mode92;
    unsigned char reserved93;
    struct ResourceGroupObject *object;
    unsigned char flag98;
    unsigned char reserved99[3];
    int value9c;
    int valuea0;
    float valuea4;
    int valuea8;
    int valueac;
    struct RenderEntryState *next;
    int valueb4;
    float depth;
    unsigned valuebc;
    unsigned char active;
    unsigned char reservedc1[3];
    int indexc4;
    int indexc8;
};

struct RenderPool {
    unsigned char reserved00[0x0c];
    struct RenderEntryState *entries;
    unsigned char reserved10[0x0c];
    int capacity;
    int used;
};

extern void *func_0019d798(unsigned heap, unsigned alignment, unsigned size);
extern void func_0018663c(struct RenderEntryState *state);

void func_00135268(unsigned heap, int capacity)
{
    struct RenderEntryState *entry;
    int i;
    struct RenderPool *const pool = &data_00242238;

    pool->capacity = capacity;
    entry = func_0019d798(heap, 4, capacity * sizeof(*entry));
    pool->used = 0;
    i = 0;
    pool->entries = entry;
    if (i < pool->capacity) {
        do {
            func_0018663c(entry);
            ++i;
            ++entry;
        } while (i < pool->capacity);
    }
}
