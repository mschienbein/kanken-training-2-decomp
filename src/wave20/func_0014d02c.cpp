#include <preinclude.h>
#include "game/display_native.h"

extern "C" void func_0014d02c(nw::lyt::Picture *pane, unsigned index,
                              const nw::math::VEC2 *coordinates)
{
    func_0014e65c(&pane->m_TexCoordAry, index, coordinates);
    if (pane->m_pMaterial != 0)
        pane->m_pMaterial->SetTextureDirty();
}
