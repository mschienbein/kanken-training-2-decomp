#include "game/memory_arena.h"
#include "game/waitable_handle.h"

/* [INFERENCE] A borrowed operation, not a second mapping-record model. */
static unsigned result_failed(CtrResult result)
{
    return (unsigned)result >> 31;
}

struct MemoryCreateOperation {
    MappedMemoryBlock *block;
    CtrResult Run(unsigned address, unsigned size, CtrMemoryPermission permissions,
        CtrMemoryPermission remote_permissions) const;
};
inline CtrResult MemoryCreateOperation::Run(unsigned address, unsigned size,
    CtrMemoryPermission permissions, CtrMemoryPermission remote_permissions) const
{
    if (block->address)
        return (CtrResult)0x08a01bf9u;
    if (address & 0xfffu)
        return (CtrResult)0xe0e01bf1u;
    if (size & 0xfffu)
        return (CtrResult)0xe0e01bf2u;
    CtrHandle handle = 0;
    CtrResult result = func_00145610(&handle, address, size, permissions, remote_permissions);
    if (!result_failed(result)) {
        CtrHandle &destination = block->handle;
        destination = handle;
        block->address = reinterpret_cast<void *>(address);
        block->size = size;
    }
    return result;
}

extern "C" void func_0014242c(MappedMemoryBlock *block, unsigned address,
    unsigned size, CtrMemoryPermission permissions, CtrMemoryPermission remote_permissions)
{
    MemoryCreateOperation operation = {block};
    CtrResult result = operation.Run(address, size, permissions, remote_permissions);
    if (result_failed(result))
        func_0019dfbc(result);
}

