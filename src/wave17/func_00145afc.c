#include "game/drawing_system.h"
struct RenderTarget;

void func_00145afc(unsigned mode)
{
    if (bss_002d3dd0.current != mode) {
        bss_002d3dd0.requested = mode;
        bss_002d3dd0.phase = 1;
    }
}
