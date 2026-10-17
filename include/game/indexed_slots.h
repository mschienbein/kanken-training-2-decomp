#ifndef GAME_INDEXED_SLOTS_H
#define GAME_INDEXED_SLOTS_H



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct IndexedSlotObject;

struct IndexedSlotObject {
    unsigned char reserved00[0x20c];
    void **slots;
};

extern "C" {
void *func_001f4d38(IndexedSlotObject *object, unsigned index, void *value);
}
#endif

#endif
