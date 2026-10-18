#ifndef GAME_REUSABLE_POOL_H
#define GAME_REUSABLE_POOL_H
#include <preinclude.h>
#include "nw/snd/snd_InstancePool.h"

/* [INFERENCE] Partial sequence-track ownership views, not historical class
 * names. The leading virtual slot destroys the object; owner is cleared before
 * returning its storage to the canonical native InstancePool. */
class ReusableObjectR41306 {
public:
    virtual ~ReusableObjectR41306();
    unsigned char reserved04[0x128];
    void *owner;
};
class ReusablePoolR41306 {
public:
    void release(ReusableObjectR41306 *object);
    const void *vtable;
    void *parser;
    nw::snd::internal::InstancePool<ReusableObjectR41306> pool;
};
#endif
