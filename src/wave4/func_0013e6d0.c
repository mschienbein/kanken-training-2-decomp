#include "game/functions.h"

struct TranslationOffsets {
    unsigned first;
    unsigned second;
};

extern struct TranslationOffsets data_00243798;

unsigned func_0013e6d0(unsigned address, unsigned size)
{
    unsigned end = address + size;
    unsigned base = func_001424d0();
    if (base <= address && address < end) {
        if (base + func_001424c0() >= end)
            return data_00243798.first + address;
    }
    return 0;
}
