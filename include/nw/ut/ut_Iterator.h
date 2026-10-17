/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/ut/ut_Iterator.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain public layouts and inline definitions; normalize types/assertion include paths for standalone release compilation.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <nw/types.h>

#include <iterator>

typedef long PtrDiff;

namespace nw{
namespace ut{
namespace internal{

template <typename TIt>
struct iterator_traits
{
    typedef typename TIt::value_type        value_type;
    typedef typename TIt::pointer           pointer;
    typedef typename TIt::reference         reference;
    typedef typename TIt::difference_type   difference_type;
    typedef typename TIt::iterator_category iterator_category;
};

template <typename T>
struct iterator_traits<T*>
{
    typedef T  value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef PtrDiff difference_type;
    typedef std::random_access_iterator_tag iterator_category;
};

template <typename T>
struct iterator_traits<const T*>
{
    typedef T           value_type;
    typedef const T*    pointer;
    typedef const T&    reference;
    typedef PtrDiff     difference_type;
    typedef std::random_access_iterator_tag iterator_category;
};


template <typename Category,typename T,typename Difference = PtrDiff,typename Pointer = T*,typename Reference = T&>
struct iterator
{
    typedef T           value_type;
    typedef Difference  difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
    typedef Category    iterator_category;
};


template <typename TIt>
class reverse_iterator : public iterator<typename iterator_traits<TIt>::iterator_category,typename iterator_traits<TIt>::value_type,typename iterator_traits<TIt>::difference_type,
typename iterator_traits<TIt>::pointer, typename iterator_traits<TIt>::reference >
{
private: 
typedef iterator<typename iterator_traits<TIt>::iterator_category,typename iterator_traits<TIt>::value_type,typename iterator_traits<TIt>::difference_type, typename iterator_traits<TIt>::pointer, typename iterator_traits<TIt>::reference >    BaseIt;

public:
    
    typedef typename BaseIt::value_type      value_type;
    typedef typename BaseIt::difference_type difference_type;
    typedef typename BaseIt::pointer         pointer;
    typedef typename BaseIt::reference       reference;
    typedef typename BaseIt::iterator_category iterator_category;

    typedef reverse_iterator<TIt>      Self;
    
    explicit reverse_iterator() {}
    explicit reverse_iterator(TIt it) : m_Current(it) {}
    
    template<typename T>
    explicit reverse_iterator(const reverse_iterator<T>& rhs ) : m_Current( rhs.GetBase() ) {}
    
    TIt GetBase() const { return m_Current; }
    reference operator*() const { TIt tmp = m_Current; return (*--tmp); }
    pointer operator->() const { return &(this->operator*()); }
    
    Self& operator++() { (void)--m_Current; return *this; }
    Self operator++(int) { Self tmp = *this; (void)--m_Current; return tmp; }
    Self& operator--() { (void)++m_Current; return *this;}
    Self operator--(int) { Self tmp = *this; (void)++m_Current; return tmp; }
    
    friend bool operator==(const Self &r1_,const Self &r2_) { return r1_.m_Current==r2_.m_Current; }
    friend bool operator!=(const Self &r1,const Self &r2) { return !(r1==r2); }
private:
    TIt m_Current;
};

} // namespace internal

}  // namespace ut
}  // namespace nw

