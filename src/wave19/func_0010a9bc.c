#include "game/drawing_system.h"


extern void func_0019b74c(int display);

void func_0010a9bc(int event)
{
    if (event == 0x400 && bss_002d3dd0.phase == 2) {
        func_0011dcec(bss_002d3dd0.requested == 0 ? 0x700 : 0x702);
        if (bss_002d3dd0.requested == 1)
            func_0019b74c(0x400);
        bss_002d3dd0.current = bss_002d3dd0.requested;
        bss_002d3dd0.phase = 0;
    }
}
