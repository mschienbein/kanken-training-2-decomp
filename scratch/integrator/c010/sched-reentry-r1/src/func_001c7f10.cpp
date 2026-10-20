#include "game/graphics_resource_abi.h"

/* The reference-tag callback clears the actual serialized target offset.
 * No opaque callback-context object is introduced. */
void func_001c7f10(nw::gfx::res::ResLookupTable object)
{
    nw::gfx::res::ResReferenceLookupTable(object.ptr()).ref().toTargetLut = 0;
}
