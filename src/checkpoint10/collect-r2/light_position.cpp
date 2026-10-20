/* SPDX-License-Identifier: GPL-3.0-only
 * Canonical NintendoWare4Ctr inline command implementation from the pinned
 * GPL-3.0 import; wrapper identity and VEC4 input remain [INFERENCE].
 */
#include <preinclude.h>
#include <nw/gfx/gfx_GraphicsDevice.h>

extern "C" void func_00143058(int index, const nw::math::VEC4& position)
{
    /* ORIGINAL BUG: Float16 overflow discards the sign and NaN payload. */
    nw::gfx::GraphicsDevice::ActivateFragmentLightPosition(index, position);
}
