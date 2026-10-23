/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from the pinned NintendoWare4Ctr public headers. The canonical
 * MoveArray constructors initialize fresh capacity storage before its setters;
 * no source-local specialization or second object model is introduced.
 * [INFERENCE] Historical constructor spelling is not recovered. */
#include <preinclude.h>
#include <nw/gfx/gfx_TransformNode.h>

namespace nw { namespace gfx {
Result TransformNode::CreateCallbacks(os::IAllocator* allocator)
{
    Result result = INITIALIZE_RESULT_OK;
    if (m_Description.isFixedSizeMemory) {
        if (m_Description.maxCallbacks == 0)
            m_PostCalculateWorldMatrixSignal = CalculateMatrixSignal::CreateInvalidateSignal(allocator);
        else
            m_PostCalculateWorldMatrixSignal = CalculateMatrixSignal::CreateFixedSizedSignal(m_Description.maxCallbacks, allocator);
    } else {
        m_PostCalculateWorldMatrixSignal = CalculateMatrixSignal::CreateVariableSizeSignal(allocator);
    }
    if (m_PostCalculateWorldMatrixSignal == NULL)
        result |= Result::MASK_FAIL_BIT;
    return result;
}
} }
