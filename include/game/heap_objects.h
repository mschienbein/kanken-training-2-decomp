#ifndef GAME_HEAP_OBJECTS_H
#define GAME_HEAP_OBJECTS_H

struct LargeHeapObject;
struct HeapObjectVTable;

#include "game/runtime.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */

struct LargeHeapObject {
    const HeapObjectVTable *vtable;
    unsigned char reserved04[0x674];
    unsigned field678;
    unsigned field67c;
};

extern "C" {

extern const HeapObjectVTable data_00223d4c;
void func_001c5b68(LargeHeapObject *object);
LargeHeapObject *func_001dadec(LargeHeapObject *object);
void func_001ce66c(LargeHeapObject *object);
extern const HeapObjectVTable data_00224bf8;
}
#endif

#endif
