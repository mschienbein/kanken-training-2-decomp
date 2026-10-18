#include "game/runtime.h"

struct AllocatorLink { AllocatorLink *previous, *next; };
struct AllocatorBase {
    const unsigned *vtable;
    unsigned char reserved04[8];
    unsigned outstanding;
    AllocatorLink *children;
    unsigned reserved14;
    ~AllocatorBase();
};
struct HeapBacking { void *handle; unsigned char reserved04[0x38]; };
/* INTEGRATOR: differs from include/game/exp_heap.h: the native destructor
 * view exposes the allocator prefix without changing arena offsets. */
struct HeapArena : AllocatorBase {
    HeapBacking backing;
    unsigned allocations;
    unsigned char *lower;
    unsigned size;
    ~HeapArena();
};
struct HeapRange { unsigned char *lower; unsigned size; };
struct ManagerAnchor {
    unsigned char reserved[0x630];
    ~ManagerAnchor();
};
struct HeapManagerBase {
    const unsigned *vtable;
    ManagerAnchor anchor;
    unsigned char active;
    unsigned char reserved635[0xb];
    HeapArena primary0, primary1, primary2;
    unsigned reserved760[2];
    HeapRange ranges[3];
    ~HeapManagerBase();
};
struct HeapManager : HeapManagerBase {
    HeapArena extra0, extra1;
    ~HeapManager();
};

/* INTEGRATOR: move to include/game/heap.h */
extern "C" {
extern const unsigned data_00223bcc[];
extern const unsigned data_00224da4[];
void func_0019dd28(HeapArena *arena);
}

__forceinline HeapArena::~HeapArena()
{
    vtable = data_00223bcc;
    func_0019dd28(this);
}

__forceinline HeapManagerBase::~HeapManagerBase()
{
    vtable = data_00224da4;
    func_0019dd28(&primary0);
    func_0019dd28(&primary1);
    func_0019dd28(&primary2);
    active = 0;
    /* [INFERENCE] callee identity: the anchor destructor is inlined by armlink. */
}

HeapManager::~HeapManager()
{
}
