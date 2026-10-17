#define NW_RELEASE
#include <nw/ut/ut_HeapBase.h>
#include <new>

/* INTEGRATOR: move to include/game/runtime_type_initializers.h */
extern "C" {
void func_0020e390(void);
}

/* The startup entry constructs the canonical root heap list in its existing
 * storage. No destructor registration is present at this entry. */
void func_0020e390(void)
{
    new (&nw::ut::internal::g_RootList) nw::ut::HeapBase::HeapList;
}
