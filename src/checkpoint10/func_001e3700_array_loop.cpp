#include <stddef.h>
#include "game/runtime.h"

struct W15fVTable;

/* INTEGRATOR: differs from src/wave6/func_0019dc20.c: the constructor
 * identifies the vtable and six words inside HeapArena.reserved00. */
struct HeapArena {
    const W15fVTable *vtable;
    unsigned int state04;
    unsigned int state08;
    unsigned int state0c;
    unsigned int state10;
    unsigned int state14;
    unsigned int state18;
    unsigned char reserved1c[0x38];
    unsigned allocations;
    unsigned char *lower;
    unsigned size;

    void initialize();
};

struct HeapRange {
    unsigned char *lower;
    unsigned size;
};

/* INTEGRATOR: differs from src/wave6/func_0019dc20.c: the constructor
 * identifies the vtable and a byte flag inside HeapManager.reserved00. */
struct HeapManager {
    const W15fVTable *vtable;
    unsigned char reserved04[0x630];
    unsigned char initialized;
    unsigned char reserved635[0x0b];
    HeapArena primary[3];
    unsigned reserved760[2];
    HeapRange ranges[3];
    HeapArena extra[2];
};

/* INTEGRATOR: move to include/game/static_objects.h */
extern "C" {
extern const W15fVTable data_00224da4;
extern const W15fVTable data_00223c14;
HeapManager *func_001e3700(HeapManager *heap);
}

inline void HeapArena::initialize()
{
    state04 = 0;
    state08 = 0;
    state0c = 0;
    state10 = 0;
    state14 = 0;
    state18 = 0;
    allocations = 0;
    vtable = &data_00223c14;
}

HeapManager *func_001e3700(HeapManager *heap)
{
    heap->vtable = &data_00224da4;
    /* [INFERENCE] callee identity: inlined by armlink; empty member initializer. */
    heap = reinterpret_cast<HeapManager *>(
        static_cast<unsigned char *>(func_001ef884(heap->reserved04))
        - offsetof(HeapManager, reserved04));
    heap->initialized = 0;
    for (unsigned i = 0; i != 3; ++i)
        heap->primary[i].initialize();
    heap->primary[2].lower = 0;
    heap->primary[2].size = 0;
    return heap;
}
