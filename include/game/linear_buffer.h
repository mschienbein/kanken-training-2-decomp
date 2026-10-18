#ifndef GAME_LINEAR_BUFFER_H
#define GAME_LINEAR_BUFFER_H

struct BufferAllocator;
struct LinearBuffer {
    struct BufferAllocator *allocator;
    unsigned position;
    void *buffer;
    unsigned capacity;
};

#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] Possible armlink-inlined allocator-field initializer; identity unknown. */
struct LinearBuffer *InitializeLinearBufferAllocator(
    struct LinearBuffer *owner, struct BufferAllocator *allocator);
struct LinearBuffer *func_00112628(struct LinearBuffer *owner,
    struct BufferAllocator *allocator, void *buffer, unsigned capacity);
struct LinearBuffer *func_00112b5c(struct LinearBuffer *owner,
    struct BufferAllocator *allocator, void *buffer, unsigned capacity);
struct LinearBuffer *func_001131d4(struct LinearBuffer *owner,
    struct BufferAllocator *allocator, void *buffer, unsigned capacity);
#ifdef __cplusplus
}
#endif

#endif
