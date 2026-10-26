/* SPDX-License-Identifier: GPL-3.0-only
 * Native SceneEnvironment reset, adapted from the pinned NintendoWare4Ctr
 * class and the retained reset operation. No storage overlay. */
#include <preinclude.h>
#include <nw/gfx/gfx_SceneEnvironment.h>

namespace nw { namespace gfx {
inline void SceneEnvironment::ResetFragmentLights()
{
    int index = 0;
    while (index < m_ActiveFragmentLightCount) {
        int slot = index++;
        m_FragmentLights[slot] = 0;
    }
    m_ActiveFragmentLightCount = 0;
    m_FragmentLightsDirty = true;
}
inline void SceneEnvironment::ResetVertexLights()
{
    int index = 0;
    if (m_ActiveVertexLightCount > 0) {
        do {
            int slot = index++;
            m_VertexLights[slot] = 0;
        } while (index < m_ActiveVertexLightCount);
    }
    m_ActiveVertexLightCount = 0;
    m_VertexLightsDirty = true;
}
void SceneEnvironment::Reset()
{
    m_AmbientLight = 0;
    m_LightSetIndex = -1;
    m_AmbientLightDirty = true;
    m_HemiSphereLight = 0;
    m_HemiSphereLightDirty = true;
    ResetFragmentLights();
    ResetVertexLights();
    m_Fog = 0;
    m_FogDirty = true;
    m_Camera = 0;
    m_CameraIndex = -1;
}
} }
