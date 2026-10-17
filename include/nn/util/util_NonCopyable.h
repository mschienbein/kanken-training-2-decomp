/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned C++03 non-copyable mixin, from public NW4C's base-class usage.
 * This is the standard private-copy idiom, not Nintendo SDK source. */
#pragma once
namespace nn { namespace util {
template<class T> class NonCopyable {
protected:
    NonCopyable() {}
    ~NonCopyable() {}
private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};
namespace ADLFireWall { using nn::util::NonCopyable; }
} }
