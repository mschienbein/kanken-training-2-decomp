#include "game/result.h"

struct ResourceSize {
    unsigned low, high;
    unsigned long long get() const { return ((unsigned long long)high << 32) | low; }
    void set(unsigned long long value) { low = (unsigned)value; high = (unsigned)(value >> 32); }
};

/* INTEGRATOR: differs from include/game/resource.h: virtual slot01 is a write
 * method with an output count, 64-bit offset, source, size and option input. */
class Resource {
public:
    virtual unsigned read(unsigned *transferred, unsigned long long offset,
        void *buffer, unsigned size);
    virtual Result write(int *transferred, long long offset,
        const void *buffer, unsigned size, int options);
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

/* INTEGRATOR: move to include/game/resource.h */
extern "C" {
void func_001211b8(int mode);
Result func_001c4a8c(int *transferred, Resource *resource,
    long long offset, const void *buffer, unsigned size, int options);
}

Result func_001c4a8c(int *transferred, Resource *resource,
    long long offset, const void *buffer, unsigned size, int options)
{
    Result result;
    if (transferred != 0 && resource != 0 && buffer != 0) {
        func_001211b8(0);
        result = resource->write(transferred, offset, buffer, size, options);
    } else {
        result = Result(0xe0e046bcu);
    }
    return result;
}
