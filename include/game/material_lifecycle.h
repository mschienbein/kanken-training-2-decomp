#ifndef GAME_MATERIAL_LIFECYCLE_H
#define GAME_MATERIAL_LIFECYCLE_H
#include "game/layout.h"
#include "game/shader.h"
#include "game/runtime.h"

/* [INFERENCE] Partial retail material owner, not an asserted identity with
 * the public nw::lyt::Material. Only the vptr, unbinding slot and storage at
 * +0x34 are recovered. C and C++ share this one ABI declaration: native C++
 * has its implicit vptr; the C deleting wrapper names that address point. */
struct MaterialObject;
struct MaterialLifecycleVTable {
    void (*operation0)(struct MaterialObject *object);
    void (*operation1)(struct MaterialObject *object);
    void (*operation2)(struct MaterialObject *object);
    void (*unbind_animation)(struct MaterialObject *object,
                             const struct LayoutAnimationTransform *animation);
};
struct MaterialObject {
#ifdef __cplusplus
    virtual void operation0();
    virtual void operation1();
    virtual void operation2();
    virtual void unbind_animation(const LayoutAnimationTransform *animation);
#else
    const struct MaterialLifecycleVTable *vtable;
#endif
    unsigned char reserved04[0x30];
    void *storage;
#ifdef __cplusplus
    ~MaterialObject();
#endif
};
#ifdef __cplusplus
extern "C" {
#endif
extern const struct MaterialLifecycleVTable data_00224480;
void func_001cfe24(struct MaterialObject *object);
#ifdef __cplusplus
}
#endif
#endif
