/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/ut/ut_ResArrayTypes.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../types.h"
#include "ut_ResArray.h"
#include "ut_ResDictionary.h"
#include <iterator>

namespace nw { 
namespace ut {

namespace internal {

template <typename TDic>
class ResArrayDicTraits
{
public:
    template <typename T>
    class Traits
    {
    public:
        typedef typename TDic::ResDicNodeData*         pointer;
        typedef const typename TDic::ResDicNodeData*   const_pointer;
        typedef T                                      reference;
        typedef const T                                const_reference;
        typedef int                                    difference_type;
        typedef std::input_iterator_tag iterator_category;

        typedef ResPtr<T>           iter_pointer;
        typedef ResPtr<const T>     const_iter_pointer;
            
        static iter_pointer        GetPointer(pointer ptr) { return ResPtr<T>( ptr->to_ptr() ); }
        static const_iter_pointer  GetPointer(const_pointer ptr) { return ResPtr<T>( ptr->to_ptr() ); }
            
        static reference    GetValue(void* ptr)   { return GetValue( reinterpret_cast<pointer>(ptr) ); }
        static reference    GetValue(pointer ptr) { return T( ptr->ofsData.to_ptr() ); }
        static const_reference GetValue(const void* ptr) { return GetValue( reinterpret_cast<const_pointer>(ptr) ); }
        static const_reference GetValue(const_pointer ptr) { return T( ptr->ofsData.to_ptr() ); }
            
        static pointer       GetNext(pointer ptr) { return ptr + 1; }
        static const_pointer GetNext(const_pointer ptr) { return ptr + 1; }
        static size_t ValueSize() { return sizeof(typename TDic::ResDicNodeData); }
    };
};

}


template <typename T>
class ResArrayPrimitive
{
public:
    typedef internal::ResArray< T, internal::ResArrayPrimitiveTraits >   type;
};

template <typename T>
class ResArrayClass
{
public:
    typedef internal::ResArray< T, internal::ResArrayClassTraits >   type;
};

template <typename T>
class ResArrayLinear
{
public:
    typedef internal::ResArray< T, internal::ResArrayDicTraits<ResDicLinearData>::Traits > type;
};

template <typename T>
class ResArrayPatricia
{
public:
    typedef internal::ResArray< T, internal::ResArrayDicTraits<ResDicPatriciaData>::Traits > type;
};


}
}

