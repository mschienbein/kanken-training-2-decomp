#ifndef GAME_HEAP_ALLOCATORS_H
#define GAME_HEAP_ALLOCATORS_H

struct HeapFactoryConfig8;
struct HeapObjectFactory;
struct HeapFactoryConfig4;
#ifdef __cplusplus
class HeapAllocatorDcb0;
class HeapAllocatorDce8;
class HeapAllocatorDd20;
class HeapAllocatorD500;
class HeapAllocator6be4;
#endif

#include "game/heap_interface.h"
#include <new>

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */





class HeapAllocatorDcb0 {
public:
    virtual ~HeapAllocatorDcb0();
    Heap *heap;
    HeapAllocatorDcb0(Heap *owner) : heap(owner) {}
};
class HeapAllocatorDce8 {
public:
    virtual ~HeapAllocatorDce8();
    Heap *heap;
    HeapAllocatorDce8(Heap *owner) : heap(owner) {}
};
class HeapAllocatorDd20 {
public:
    virtual ~HeapAllocatorDd20();
    Heap *heap;
    HeapAllocatorDd20(Heap *owner) : heap(owner) {}
};
class HeapAllocatorD500 {
public:
    virtual ~HeapAllocatorD500();
    Heap *heap;
    void *resource;
    HeapAllocatorD500(Heap *owner) : heap(owner), resource(0) {}
};
class HeapAllocator6be4 {
public:
    virtual ~HeapAllocator6be4();
    Heap *heap;
    HeapAllocator6be4(Heap *owner) : heap(owner) {}
};

extern "C" {

HeapAllocatorDcb0 *func_0010dcb0(HeapFactoryConfig8 *config, Heap *heap);

HeapAllocatorDce8 *func_0010dce8(Heap *heap);

HeapAllocatorDd20 *func_0010dd20(HeapFactoryConfig4 *config, Heap *heap);
HeapAllocatorD500 *func_0010d500(Heap *heap);
HeapAllocator6be4 *func_00116be4(Heap *heap);
}
#endif

#endif
