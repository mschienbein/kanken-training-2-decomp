#ifndef GAME_MEMORY_ARENA_H
#define GAME_MEMORY_ARENA_H
#include "ctr/svc.h"
#include "game/stream_manager.h"

/* The map/unmap paths pass the same intrusive allocation record. */
struct MappedMemoryBlock {
    struct MappedMemoryBlock *next, *previous;
    void *address;
    unsigned size;
    unsigned char read_only;
    unsigned char reserved11[3];
    CtrHandle handle;
    unsigned char mapped;
};
struct MemoryArena {
    unsigned char *begin, *end;
    struct MappedMemoryBlock *current;
    struct StreamMutex mutex;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct MemoryArena bss_004b51d0, bss_004b204c;
extern unsigned char data_00242d74;
extern void func_0011073c(struct MappedMemoryBlock *block, unsigned size);
extern void func_001927b8(struct MemoryArena *arena, void *memory, unsigned size);
extern void func_00148004(struct MappedMemoryBlock *block);
extern void func_0014ae4c(struct MemoryArena *arena, struct MappedMemoryBlock *block);
extern void *func_001906c8(struct MemoryArena *arena, struct MappedMemoryBlock *block,
                           unsigned size, unsigned alignment);
#ifdef __cplusplus
}
#endif

#endif
