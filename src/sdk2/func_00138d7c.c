#include "game/service_state.h"

/* Same generation-bearing input and list views as src/wave6/func_00112c98.c. */
struct ServiceResetState { unsigned int generation; };
struct ServiceEntry {
    unsigned int value;
    unsigned char kind, variant;
    unsigned short attributes;
};
struct ServiceList {
    struct ServiceEntry *entries;
    unsigned int capacity;
    unsigned int count;
    unsigned int remainder[36];
};

unsigned func_00138d7c(const struct ServiceResetState *object, register unsigned *entry_count)
{
    void *memory;
    unsigned result = func_0018fa44(func_0018fcb4(), &memory, object->generation);
    switch (result >> 31) {
    case 0: *entry_count = ((const struct ServiceList *)memory)->count; return 0;
    default: return result;
    }
}
