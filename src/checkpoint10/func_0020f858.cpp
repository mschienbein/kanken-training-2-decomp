#include "game/runtime.h"
#include "game/screen.h"
#include <stddef.h>


void func_0020f858(void)
{
    bss_00246b48.first = new (data_002232d5) NameTable(12, 49, "Qst_Dlg_Crs_00", 0);
    bss_00246b48.table = new (data_002232d5) NameTable(12, 16, "Qst_Dlg_Frm_Ans_00", 8,
        "Qst_Dlg_Frm_Ans_00_none", "Qst_Dlg_Frm_Ans_00_01", "Qst_Dlg_Frm_Ans_00_02",
        "Qst_Dlg_Frm_Ans_00_03", "Qst_Dlg_Frm_Ans_00_04", "Qst_Dlg_Frm_Ans_00_05",
        "Qst_Dlg_Frm_Ans_00_06", "Qst_Dlg_Frm_Ans_00_07");
    bss_00246b48.terminator = new (data_002232d5) NameTable;
}
