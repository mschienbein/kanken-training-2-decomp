#include "game/runtime.h"

/* INTEGRATOR: move to include/game/package_text.h */
extern "C" {
extern unsigned func_001949e4(unsigned short *destination, int *destination_count,
                              const char *source, int *source_count);
extern unsigned short *func_0018651c(unsigned short *destination,
                                    const unsigned short *source, unsigned count);
extern void func_0019e08c(void *allocation);
unsigned func_00193168(unsigned short *destination, unsigned capacity, const char *source);
}

/* Reference-selecting minimum: the address of the first operand stays the default. */
static inline unsigned minimum(const unsigned &a, const unsigned &b)
{
    const unsigned *smaller = &a;
    if (a >= b)
        smaller = &b;
    return *smaller;
}

unsigned func_00193168(unsigned short *destination, unsigned capacity, const char *source)
{
    int required;
    int source_count = -1;
    unsigned result = func_001949e4(0, &required, source, &source_count);
    bool failed = (result >> 31) != 0;
    if (failed)
        return result;
    unsigned short *temporary = static_cast<unsigned short *>(
        func_00193110((required + 1) * 2, data_002232d5));
    result = func_001949e4(temporary, &required, source, &source_count);
    if (result & 0x80000000u) {
        func_0019e08c(temporary);
        return result;
    }
    result = 0;
    temporary[required] = 0;
    func_0018651c(destination, temporary,
                  minimum(capacity, static_cast<unsigned>(required + 1)));
    /* ORIGINAL BUG: a zero capacity writes before the destination. */
    destination[capacity - 1] = 0;
    func_0019e08c(temporary);
    return result;
}
