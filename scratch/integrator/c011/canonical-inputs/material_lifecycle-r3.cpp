/* SPDX-License-Identifier: GPL-3.0-only
 * Canonical native Material lifecycle from the matching retail destructor
 * and the pinned NintendoWare4Ctr declaration, 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified: use the recovered project allocator endpoint and natural virtual
 * unbinding helper instead of a parallel MaterialObject/ResourceCollection.
 */
#include <preinclude.h>
#include "nw/lyt/lyt_Material.h"
#include "game/shader.h"

namespace nw { namespace lyt {
static inline void unbind_material(Material *object)
{
    object->UnbindAnimation(0);
}

Material::~Material()
{
    unbind_material(this);
    if (m_pMem) {
        func_00191aec(m_pMem);
        m_pMem = 0;
    }
}
} }
