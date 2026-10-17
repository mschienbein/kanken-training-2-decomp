#include "game/runtime.h"
#include "game/screen.h"
#include <stddef.h>

struct NameTableVTable;
/* Canonical NameTable from include/game/screen.h. */

/* INTEGRATOR: differs from include/game/screen.h: this initializer writes
 * descriptor pointers in all three slots at +0/+4/+8. */
struct AnimationContext {
    const NameTable *first;
    const NameTable *table;
    const NameTable *terminator;
};
void *operator new(size_t size, const void *tag) throw();
/* INTEGRATOR: move to include/game/property_descriptor.h after correcting
 * AnimationContext in include/game/screen.h. */
extern "C" {


void func_0020f858(void);
}

void func_0020f858(void)
{
    bss_00246b48.first = new (data_002232d5) NameTable(12, 49, "Qst_Dlg_Crs_00", 0);
    bss_00246b48.table = new (data_002232d5) NameTable(12, 16, "Qst_Dlg_Frm_Ans_00", 8,
        "Qst_Dlg_Frm_Ans_00_none", "Qst_Dlg_Frm_Ans_00_01", "Qst_Dlg_Frm_Ans_00_02",
        "Qst_Dlg_Frm_Ans_00_03", "Qst_Dlg_Frm_Ans_00_04", "Qst_Dlg_Frm_Ans_00_05",
        "Qst_Dlg_Frm_Ans_00_06", "Qst_Dlg_Frm_Ans_00_07");
    bss_00246b48.terminator = new (data_002232d5) NameTable;
}
