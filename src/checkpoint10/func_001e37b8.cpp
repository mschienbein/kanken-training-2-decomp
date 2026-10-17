/* This is the allocation slot of the Heap interface used by game_heap.cpp.
 * The alignment input is retained even though this implementation ignores it. */
class Heap {
public:
    virtual ~Heap();
    virtual void *allocate(unsigned int size, unsigned int alignment);
};

struct HeapManager;

/* INTEGRATOR: move to include/game/heap_allocation.h */
extern "C" {
extern HeapManager bss_004952f0;
/* [INFERENCE] Names of the three fixed allocator-selection words. */
void *func_001e35e4(HeapManager *manager, unsigned int region,
                   unsigned int mode, unsigned int flags, unsigned int size);
void *func_001e37b8(Heap *allocator, unsigned int size, unsigned int alignment);
}

void *func_001e37b8(Heap *allocator, unsigned int size, unsigned int alignment)
{
    return func_001e35e4(&bss_004952f0, 0x10000, 0x100, 0, size);
}
