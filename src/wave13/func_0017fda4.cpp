#include <preinclude.h>
#include <nw/lyt/lyt_Picture.h>
#include <nw/lyt/lyt_Layout.h>

namespace nw { namespace lyt {
void Picture::SetMaterial(Material *material)
{
    Material *previous = m_pMaterial;
    if (previous == material)
        return;
    if (previous && !previous->IsUserAllocated()) {
        previous->~Material();
        Layout::FreeMemory(previous);
    }
    m_pMaterial = material;
    if (material)
        material->SetTextureDirty();
}
} }
