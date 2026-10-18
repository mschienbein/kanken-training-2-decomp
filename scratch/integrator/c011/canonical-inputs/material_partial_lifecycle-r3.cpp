#include "game/layout.h"
#include "game/shader.h"
#include "game/runtime.h"

/* [INFERENCE] Retail material lifecycle view, not an asserted identity with
 * the public nw::lyt::Material whose complete RTTI does not place here.
 * The first three virtual operation roles and full historical extent remain
 * unrecovered; the retained fourth slot unbinds an animation pointer. */
class MaterialObject {
public:
    virtual void operation0();
    virtual void operation1();
    virtual void operation2();
    virtual void unbind_animation(const LayoutAnimationTransform *animation);
    unsigned char reserved04[0x30];
    void *storage;
    ~MaterialObject();
};

static inline void unbind_material(MaterialObject *object)
{
    object->unbind_animation(0);
}

MaterialObject::~MaterialObject()
{
    unbind_material(this);
    if (storage) {
        func_00191aec(storage);
        storage = 0;
    }
}

extern "C" void func_001cfe24(MaterialObject *object)
{
    object->~MaterialObject();
    func_001852f4(object);
}
