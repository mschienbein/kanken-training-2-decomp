/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_InstancePool.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_INSTANCE_POOL_H_
#define NW_SND_INSTANCE_POOL_H_

#include <new>
#include "snd_Global.h"

namespace nw { 
namespace snd { 
namespace internal {

class PoolImpl
{
public:
    PoolImpl(): 
        m_pNext(NULL)
    {
    }

protected:
    u32 CreateImpl(void* buffer, size_t size, u32 objSize);
    void DestroyImpl(void* buffer, size_t size);
    int CountImpl() const;
    void* AllocImpl();
    void FreeImpl(void* ptr);

private:
    PoolImpl* m_pNext;
};

template <typename T>
class InstancePool : private PoolImpl
{
public:
    u32 Create(void* buffer, unsigned long size)
    {
        u32 objSize =
            (sizeof(T) > sizeof(InstancePool<T>*))
                ? sizeof(T)
                : sizeof(InstancePool<T>*);
        return CreateImpl(buffer, size, objSize);
    }

    void Destroy(void* buffer, size_t size)
    {
        DestroyImpl(buffer, size);
    }

    int Count() const
    {
        return CountImpl();
    }

    T* Alloc()
    {
        void *ptr = AllocImpl();
        if (ptr == NULL)
            return NULL;
        return new (ptr) T();
    }

    void Free(T* obj)
    {
        if (obj == NULL)
            return;
        obj->~T();
        FreeImpl(obj);
    }
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_INSTANCE_POOL_H_

