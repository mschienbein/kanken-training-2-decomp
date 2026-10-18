/* [INFERENCE] Descriptive exclusive-word wrapper. The signed state and scalar
 * success contract follow the recursive-mutex client in src/sdk/func_0010be14.c.
 * ARMCC __ldrex, __strex and __clrex are approved compiler intrinsics. */
#include "game/synchronization.h"

class ExclusiveWord {
    unsigned *value;
public:
    ExclusiveWord(unsigned *address) : value(address) {}
    unsigned load() { return __ldrex(value); }
    bool store(unsigned replacement) { return __strex(replacement, value) == 0; }
};

int func_0011359c(int *state)
{
    ExclusiveWord word((unsigned *)state);
    for (;;) {
        int value = (int)word.load();
        if (value > 0) {
            if (word.store(0u - (unsigned)value))
                return 1;
        } else {
            __clrex();
            return 0;
        }
    }
}
