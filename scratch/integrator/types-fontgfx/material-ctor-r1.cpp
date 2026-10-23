/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from pinned NintendoWare4Ctr lyt_Material.cpp, commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
#include <nw/lyt/lyt_Material.h>
#include <cstring>

namespace nw { namespace lyt {
inline void Material::InitMatMemNums(internal::MatMemNums *numbers)
{
    numbers->texMap = 0;
    numbers->texSRT = 0;
    numbers->texCoordGen = 0;
    numbers->tevStage = 0;
    numbers->alpComp = 0;
    numbers->blendMode = 0;
}
inline void Material::Init()
{
    m_Colors[0] = ut::Color8(0x00000000);
    m_Colors[1] = ut::Color8(0xffffffff);
    m_Colors[2] = ut::Color8(0xffffffff);
    InitMatMemNums(&m_MemCap);
    InitMatMemNums(&m_MemNum);
    m_Flag = 0;
    m_pMem = 0;
}
Material::Material()
{
    Init();
    std::memset(m_Name, 0, sizeof m_Name);
}
} }
