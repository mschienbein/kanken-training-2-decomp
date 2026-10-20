#include <preinclude.h>
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Util.h>

namespace nw { namespace lyt {
AlphaCompare *Material::GetAlphaComparePtr()
{
    /* ReserveMem constructs AlphaCompare after the three typed arrays. */
    unsigned offset = m_MemCap.texMap * sizeof(TexMap) +
        m_MemCap.texSRT * sizeof(TexSRT) +
        m_MemCap.texCoordGen * sizeof(TexCoordGen);
    return internal::ConvertOffsToPtr<AlphaCompare>(m_pMem, offset);
}
} }
