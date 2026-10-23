#include "game/command_state.h"
#include "game/exp_heap.h"
#include "game/playback_screen.h"
#include "game/resource_tables.h"



#include "game/font.h"


void func_00145928(uintptr_t resource, int index)
{
    bss_002d3d60.firstEnabled = 1;
    bss_002d3d60.firstResource = resource;
    bss_002d3d60.firstIndex = index;
    func_00181a4c(resource, index, index, 3, 128);
    func_00186598();
    func_001811b8();
    bss_002d3d60.firstSheets = func_0019d798(2, 4,
        func_0019d8c4((const struct FontFile *)func_001971a0(resource, index)));
}
