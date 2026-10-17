#ifndef GAME_RESOURCE_H
#define GAME_RESOURCE_H
#include "game/resource_owner.h"

/* Reference-owned resource interface (C++ only; partial vtable). */

#ifndef __cplusplus
#error "game/resource.h is C++ only"
#endif

#include "game/result.h"

struct ResourceSize {
    unsigned low;
    unsigned high;
    unsigned long long get() const { return ((unsigned long long)high << 32) | low; }
    void set(unsigned long long value) { low = (unsigned)value; high = (unsigned)(value >> 32); }
};

class Resource {
public:
    virtual unsigned read(unsigned *transferred, unsigned long long offset,
                          void *buffer, unsigned size);
    virtual void slot01();
    virtual void slot02();
    virtual Result get_size(ResourceSize *size);
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void slot10();
    virtual void slot11();
    virtual void release();
};

/* Owner holding a tagged resource pointer: bit 0 flags an unexpected state on release. */


extern "C" void func_0019d5c4(Resource *resource);

#endif
