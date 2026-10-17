#include "game/resource_tables.h"

extern unsigned char *func_0019dc04(void);

int func_00181aac(int selection)
{
    unsigned char *resource;
    void *sheets;
    switch (selection) {
    case 0:
        resource = func_0019dc04();
        sheets = bss_002d3d60.defaultSheets;
        break;
    case 1:
        if (bss_002d3d60.firstEnabled != 1)
            return 0;
        resource = func_001971a0(bss_002d3d60.firstResource, bss_002d3d60.firstIndex);
        sheets = bss_002d3d60.firstSheets;
        break;
    case 2:
        if (bss_002d3d60.secondEnabled != 1)
            return 0;
        resource = func_001971a0(bss_002d3d60.secondResource, bss_002d3d60.secondIndex);
        sheets = bss_002d3d60.secondSheets;
        break;
    default:
        return 0;
    }
    if (!resource)
        return 0;
    func_0019dbec(bss_002d3d60.font);
    func_0019da14(bss_002d3d60.font, (struct FontFile *)resource);
    func_0019d9d8(bss_002d3d60.font, sheets);
    return 1;
}
