#include "game/layout.h"

class LayoutAnimationChild {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void detach(const LayoutAnimationTransform *animation);
};
class LayoutAnimationOwner {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual unsigned childCount() const;
    virtual LayoutAnimationChild *child(unsigned index);
    unsigned char reserved04[0x18];
    LayoutAnimationList animations;
};

extern "C" void func_0016b58c(LayoutAnimationOwner *owner,
                              const LayoutAnimationTransform *animation)
{
    unsigned count = owner->childCount();
    for (unsigned index = 0; index < count; ++index) {
        LayoutAnimationChild *child = owner->child(index);
        if (child)
            child->detach(animation);
    }
    func_0016b5fc(&owner->animations, animation);
}
