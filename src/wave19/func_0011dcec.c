#include "game/bss_0049ecf0.h"
struct CommandQueue;
struct FramebufferSurface;

void func_0011dcec(int mode)
{
    if (bss_0049ecf0.display_mode == mode)
        return;
    switch (mode) {
    case 0x700:
        bss_0049ecf0.display_heights[0] = 400;
        break;
    case 0x701:
        bss_0049ecf0.display_heights[0] = 800;
        break;
    case 0x702:
        bss_0049ecf0.display_heights[0] = 400;
        break;
    default:
        return;
    }
    bss_0049ecf0.display_mode = mode;
}
