#ifndef GAME_GRAPHICS_RESOURCES_H
#define GAME_GRAPHICS_RESOURCES_H
#include "game/matrix.h"
#include "game/runtime.h"
struct GraphicsRenderer;
struct GraphicsResource;
struct GraphicsHeap;
struct GraphicsDescription;
struct GraphicsResources {
    BufferState buffers;
    unsigned reserved0c;
    struct GraphicsRenderer *renderer;
    unsigned field14, field18, field1c, field20, field24;
    struct GraphicsResource *resources[3];
};

struct TextureResource;
struct GraphicsResourceLink {
    struct TextureResource *resource;
    unsigned kind, identifier;
    struct GraphicsResourceLink *next;
};
struct SlotValue { void *value; };
struct GraphicsResourceTable {
    struct TextureResource *defaults[2];
    unsigned reserved08[2];
    struct GraphicsResourceLink *buckets[512];
    unsigned char reserved810[0x18];
    const struct SlotValue *slots[32];
};

#ifdef __cplusplus
struct GraphicsHeap;
struct GraphicsHeapVTable {
    unsigned char reserved00[8];
    void *(*allocate)(GraphicsHeap *heap, unsigned size, unsigned alignment);
    void (*release)(GraphicsHeap *heap, void *allocation);
};

struct GraphicsHeap {
    const GraphicsHeapVTable *vtable;
};

struct GraphicsResource;
struct GraphicsResourceVTable {
    void (*destroy)(GraphicsResource *resource);
};
struct GraphicsResource {
    const GraphicsResourceVTable *vtable;
    GraphicsHeap *heap;
    unsigned char reserved08[0x140];
    Matrix34 view;
    unsigned char reserved178[0x30];
    Matrix44 projection;
    unsigned char reserved1e8[0xac];
    float depth_offset;
};

struct GraphicsReference {
    GraphicsResource *pointer;
    bool owning;

    __forceinline GraphicsReference() : pointer(0), owning(true) {}
    __forceinline ~GraphicsReference()
    {
        if (pointer != 0)
            release(pointer, owning);
    }

    static void release(GraphicsResource *resource, bool owned)
    {
        if (owned) {
            resource->vtable->destroy(resource);
            resource->heap->vtable->release(resource->heap, resource);
        }
    }
};

struct GraphicsDescription {
    bool enabled;
    bool buffered;
    unsigned word04;
    unsigned word08;
    unsigned mode;
    GraphicsReference first;
    GraphicsReference second;

    __forceinline GraphicsDescription() : enabled(true), buffered(true), mode(1)
    {
        word08 = 0;
        word04 = 0;
    }
    __forceinline ~GraphicsDescription() {}
};

extern "C" {
#endif
extern struct GraphicsResources bss_0049da58;
extern struct GraphicsResourceTable *data_00242a0c;
extern struct GraphicsHeap data_002426d0;
extern const struct GraphicsHeapVTable data_00224db4;
extern struct GraphicsResource *func_001936a0(struct GraphicsDescription *description, struct GraphicsHeap *heap);
extern void func_00108f94(void);
#ifdef __cplusplus
}
#endif
#endif
