/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Step-3 common lifetime-helper form, isolated after the whole-TU experiment.
 */
#include <preinclude.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Animation.h>
namespace {
static void free_layout_memory(void* memory)
{
    nw::lyt::Layout::GetAllocator()->Free(memory);
}

template<typename T>
static void destroy_layout_object(T* object)
{
    if (object) {
        object->~T();
        free_layout_memory(static_cast<void*>(object));
    }
}

}

namespace nw { namespace lyt {
void Layout::DeleteAnimTransform(AnimTransform* animation)
{
    /* ORIGINAL BUG: removal precedes the object null test. */
    m_AnimTransList.Erase(animation);
    destroy_layout_object(animation);
}
} }

