#include "game/display_target.h"
/* [INFERENCE] Class and virtual-hook names; observed target dimensions and GL formats. */

DisplayTarget::DisplayTarget()
    : color_format(0x8058), width(240), height(400), flags(0x10000), depth_format(0x8ce0)
{
}
