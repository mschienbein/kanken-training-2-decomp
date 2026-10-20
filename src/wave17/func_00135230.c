#include "game/functions.h"
#include "game/resource_tables.h"



void func_00135230(void)
{
    bss_002d3d60.firstEnabled = 0;
    func_0019d95c(bss_002d3d60.firstResource, bss_002d3d60.firstIndex, 0);
    if (bss_002d3d60.firstSheets) {
        func_0019dc20(bss_002d3d60.firstSheets);
        bss_002d3d60.firstSheets = 0;
    }
}
