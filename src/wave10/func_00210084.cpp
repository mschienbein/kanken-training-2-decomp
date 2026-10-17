#include "game/pane_descriptors.h"

void func_00210084(void)
{
    data_0023ed08[0] = new (data_002232d5) PaneRange(0, 2, 26.0f, 50.0f);
    data_0023ed08[1] = new (data_002232d5) PaneRange(1, 2, 26.0f, 50.0f);
    bss_00247140[0] = new (data_002232d5) NameTable(1, 76, "Cmn_Crs_Scr_00", 1, "Cmn_Crs_Scr_00_Loop");
    bss_00247140[1] = new (data_002232d5) NameTable(1, 77, "Cmn_Crs_Scr_10", 1, "Cmn_Crs_Scr_10_Loop");
    bss_00247140[2] = new (data_002232d5) NameTable(10, 42, "Abi_Pic_Pen_00", 4, "Abi_Pic_Pen_00_Off", "Abi_Pic_Pen_00_Loop", "Abi_Pic_Pen_00_x_Scroll", "Abi_Pic_Pen_00_y_Scroll");
    bss_00247140[3] = new (data_002232d5) NameTable();
}
