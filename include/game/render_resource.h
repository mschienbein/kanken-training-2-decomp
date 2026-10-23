#ifndef GAME_RENDER_RESOURCE_H
#define GAME_RENDER_RESOURCE_H
#include "game/runtime.h"
#include "game/functions.h"

/* [INFERENCE] Shared resource-group prefix; count is not bounds-checked by
 * the original fixed-id-array lifecycle. No complete native class is claimed. */
struct RenderGroupVTable;
struct RenderResourceGroup {
    const struct RenderGroupVTable *vtable;
    unsigned char initialized;
    unsigned char reserved05[3];
    unsigned mode;
    unsigned count;
    unsigned ids[3];
    int width, height;
#ifdef __cplusplus
    void finalize();
    void clearIds();
#endif
};
struct RenderResourceConfiguration {
    unsigned reserved00, mode, count, format;
    int width, height;
    unsigned memory_type;
};

struct RendererResource;
struct ShutdownClient;
struct RendererShutdownState {
    BufferState allocation;
    unsigned state;
    struct RendererResource *resource;
    struct ShutdownClient *clients[2];
};
#ifdef __cplusplus
/* [INFERENCE] Native shutdown client identity and virtual slot name. */
#include <preinclude.h>
#include <nw/os/os_Memory.h>
#include <nw/anim/res/anim_ResAnim.h>
#include <nw/gfx/gfx_AnimGroup.h>
struct TextureBufferOwnerVTable;
/* [INFERENCE] Independent handle owner; only its descriptive emitted destructor
 * identity is bound, not a public native class or a C destructor alias. */
struct TextureBufferOwner {
    const TextureBufferOwnerVTable *vtable;
    unsigned reserved04;
    GLuint first_texture, second_texture, buffer;
    ~TextureBufferOwner();
};
extern "C" {
extern const TextureBufferOwnerVTable data_00223ecc;
void func_0016cbf4(GLsizei count, const GLuint *textures);
void func_00215cb0(GLsizei count, const GLuint *buffers);
extern const unsigned data_0022409c;
}
/* [INFERENCE] Descriptive native-member storage, not the complete public
 * AnimGroup identity. Existing D1/D2 bindings retain their extent qualifications. */
struct R41270AnimGroupStorage {
    const void *vtable;
    nw::os::IAllocator *allocator;
    nw::anim::ResAnimGroup resource;
    nw::ut::MoveArray<nw::anim::AnimBlendOp*> blend_operations;
    nw::gfx::SceneNode *owner;
    nw::ut::MoveArray<int> target_indices;
    nw::ut::MoveArray<void*> target_objects;
    nw::ut::MoveArray<void*> target_pointers;
    nw::ut::MoveArray<const void*> original_values;
    nw::gfx::AnimGroup::PreEvaluateCallback callback;
    bool full_baked;
    R41270AnimGroupStorage(nw::anim::ResAnimGroup info, nw::gfx::SceneNode *node,
                          nw::os::IAllocator *heap)
        : vtable(&data_0022409c), allocator(heap), resource(info), owner(node),
          callback(0), full_baked(false) {}
    ~R41270AnimGroupStorage();
};
struct OwnedBufferVTable;
struct OwnedBufferState {
    const OwnedBufferVTable *vtable;
    nw::os::IAllocator *allocator;
    bool owns;
    unsigned char reserved09[3];
    void *buffer;
    ~OwnedBufferState();
};
struct CachedBufferVTable;
struct CachedBufferState {
    const CachedBufferVTable *vtable;
    nw::os::IAllocator *allocator;
    bool flag08;
    bool owns;
    unsigned char reserved0a[0x12];
    void *buffer;
    ~CachedBufferState();
};
struct ShutdownClient {
    virtual ~ShutdownClient();
    virtual void shutdown();
};
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern struct RendererShutdownState bss_0049d93c;
extern struct RendererResource *func_0010e1ac(struct RendererResource *resource);
extern void func_00104abc(void);
extern const struct RenderGroupVTable data_00224e04;
extern struct RenderResourceGroup *func_0016cb6c(struct RenderResourceGroup *group);
extern void func_0019b988(const struct RenderResourceConfiguration *configuration,
                          struct RenderResourceGroup *group);
extern void func_0019cc68(unsigned id);
extern void func_0019cb3c(int count, const unsigned *ids);
extern void func_00196114(unsigned mode);
extern void func_0011dbb4(int count, unsigned *ids);
extern void func_0011dab4(unsigned format, int width, int height, unsigned memory_type);
#ifdef __cplusplus
extern const OwnedBufferVTable data_00223f3c;
extern const CachedBufferVTable data_00223f9c;
extern OwnedBufferState *func_001c6afc(OwnedBufferState *object);
extern CachedBufferState *func_001c7380(CachedBufferState *object);
#endif
#ifdef __cplusplus
}
#endif

#endif
