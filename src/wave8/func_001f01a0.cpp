#include "game/layout.h"

/* The virtual slot is used as material(pane, index) by func_001cd914,
 * func_001cd384 and func_001cd9d8. This base pane has no materials. */
extern "C" LayoutMaterial *func_001f01a0(const LayoutPane *pane, unsigned index)
{
    return 0;
}
