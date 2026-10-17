#include "game/property_descriptor.h"
#include "game/runtime.h"
#include "game/screen.h"
#include "game/startup_descriptors.h"
#include <stddef.h>
#include "game/item_pages.h"

/* Startup descriptors are allocated before their consumers are initialized.
 * The out-of-line constructors establish the descriptor vtable and owned lists.
 * [INFERENCE] Class and field names describe their observed initialization roles. */


/* ItemPageSet is partial in the shared header; this initializer allocates 0x1c. */



void func_00210964(void)
{
    bss_00246d3c[0] = new (data_002232d5) NameTable(1, 32, "Cmn_Crs_Arw_00", 2,
        "Cmn_Crs_Arw_00_Off", "Cmn_Crs_Arw_00_OnClick");
    bss_00246d3c[1] = new (data_002232d5) NameTable(1, 33, "Cmn_Crs_Arw_01", 2,
        "Cmn_Crs_Arw_01_Off", "Cmn_Crs_Arw_01_OnClick");
    bss_00246d3c[2] = new (data_002232d5) NameTable(1, 15, "Cmn_Btn_Dlg_04", 1,
        "Cmn_Btn_Dlg_04_Off");
    bss_00246d3c[3] = new (data_002232d5) NameTable(12, 20, "Qst_Bt_Up_00", 12,
        "Qst_Bt_Up_00_Bt_Up_00", "Qst_Bt_Up_00_Bt_Up_01", "Qst_Bt_Up_00_Bt_Up_02",
        "Qst_Bt_Up_00_Bt_Up_03", "Qst_Bt_Up_00_Bt_Up_04", "Qst_Bt_Up_00_Bt_Up_05",
        "Qst_Bt_Up_00_Bt_Up_06", "Qst_Bt_Up_00_Bt_Up_07", "Qst_Bt_Up_00_Bt_Up_08",
        "Qst_Bt_Up_00_Bt_Up_09", "Qst_Bt_Up_00_Bt_Up_10", "Qst_Bt_Up_00_Bt_Up_11");
    bss_00246d3c[4] = new (data_002232d5) NameTable(12, 21, "Qst_Bt_Lo_00", 31,
        "Qst_Bt_Lo_00_Bt_Lo_00", "Qst_Bt_Lo_00_Bt_Lo_01", "Qst_Bt_Lo_00_Bt_Lo_02",
        "Qst_Bt_Lo_00_Bt_Lo_03", "Qst_Bt_Lo_00_Bt_Lo_04", "Qst_Bt_Lo_00_Bt_Lo_05",
        "Qst_Bt_Lo_00_Bt_Lo_06", "Qst_Bt_Lo_00_Bt_Lo_07", "Qst_Bt_Lo_00_Bt_Lo_08",
        "Qst_Bt_Lo_00_Bt_Lo_09", "Qst_Bt_Lo_00_Bt_Lo_10", "Qst_Bt_Lo_00_Bt_Lo_11",
        "Qst_Bt_Lo_00_Bt_Lo_12", "Qst_Bt_Lo_00_Bt_Lo_13", "Qst_Bt_Lo_00_Bt_Lo_14",
        "Qst_Bt_Lo_00_Bt_Lo_15", "Qst_Bt_Lo_00_Bt_Lo_16", "Qst_Bt_Lo_00_Bt_Lo_17",
        "Qst_Bt_Lo_00_Bt_Lo_18", "Qst_Bt_Lo_00_Bt_Lo_19", "Qst_Bt_Lo_00_Bt_Lo_20",
        "Qst_Bt_Lo_00_Bt_Lo_21", "Qst_Bt_Lo_00_Bt_Lo_22", "Qst_Bt_Lo_00_Bt_Lo_23",
        "Qst_Bt_Lo_00_Bt_Lo_24", "Qst_Bt_Lo_00_Bt_Lo_25", "Qst_Bt_Lo_00_Bt_Lo_26",
        "Qst_Bt_Lo_00_Bt_Lo_27", "Qst_Bt_Lo_00_Bt_Lo_28", "Qst_Bt_Lo_00_Bt_Lo_29",
        "Qst_Bt_Lo_00_Bt_Lo_30");
    bss_00246d3c[5] = new (data_002232d5) NameTable(12, 6, "Qst_Bg_Up_06", 0);
    bss_00246d3c[6] = new (data_002232d5) NameTable();
    bss_00246d58[0] = new (data_002232d5) NameTable(1, 32, "Cmn_Crs_Arw_00", 2,
        "Cmn_Crs_Arw_00_Off", "Cmn_Crs_Arw_00_OnClick");
    bss_00246d58[1] = new (data_002232d5) NameTable(1, 33, "Cmn_Crs_Arw_01", 2,
        "Cmn_Crs_Arw_01_Off", "Cmn_Crs_Arw_01_OnClick");
    bss_00246d58[2] = new (data_002232d5) NameTable(1, 85, "Cmn_Btn_Dlg_04_h", 1,
        "Cmn_Btn_Dlg_04_h_Off");
    bss_00246d58[3] = new (data_002232d5) NameTable(12, 20, "Qst_Bt_Up_00", 12,
        "Qst_Bt_Up_00_Bt_Up_00", "Qst_Bt_Up_00_Bt_Up_01", "Qst_Bt_Up_00_Bt_Up_02",
        "Qst_Bt_Up_00_Bt_Up_03", "Qst_Bt_Up_00_Bt_Up_04", "Qst_Bt_Up_00_Bt_Up_05",
        "Qst_Bt_Up_00_Bt_Up_06", "Qst_Bt_Up_00_Bt_Up_07", "Qst_Bt_Up_00_Bt_Up_08",
        "Qst_Bt_Up_00_Bt_Up_09", "Qst_Bt_Up_00_Bt_Up_10", "Qst_Bt_Up_00_Bt_Up_11");
    bss_00246d58[4] = new (data_002232d5) NameTable(12, 21, "Qst_Bt_Lo_00", 31,
        "Qst_Bt_Lo_00_Bt_Lo_00", "Qst_Bt_Lo_00_Bt_Lo_01", "Qst_Bt_Lo_00_Bt_Lo_02",
        "Qst_Bt_Lo_00_Bt_Lo_03", "Qst_Bt_Lo_00_Bt_Lo_04", "Qst_Bt_Lo_00_Bt_Lo_05",
        "Qst_Bt_Lo_00_Bt_Lo_06", "Qst_Bt_Lo_00_Bt_Lo_07", "Qst_Bt_Lo_00_Bt_Lo_08",
        "Qst_Bt_Lo_00_Bt_Lo_09", "Qst_Bt_Lo_00_Bt_Lo_10", "Qst_Bt_Lo_00_Bt_Lo_11",
        "Qst_Bt_Lo_00_Bt_Lo_12", "Qst_Bt_Lo_00_Bt_Lo_13", "Qst_Bt_Lo_00_Bt_Lo_14",
        "Qst_Bt_Lo_00_Bt_Lo_15", "Qst_Bt_Lo_00_Bt_Lo_16", "Qst_Bt_Lo_00_Bt_Lo_17",
        "Qst_Bt_Lo_00_Bt_Lo_18", "Qst_Bt_Lo_00_Bt_Lo_19", "Qst_Bt_Lo_00_Bt_Lo_20",
        "Qst_Bt_Lo_00_Bt_Lo_21", "Qst_Bt_Lo_00_Bt_Lo_22", "Qst_Bt_Lo_00_Bt_Lo_23",
        "Qst_Bt_Lo_00_Bt_Lo_24", "Qst_Bt_Lo_00_Bt_Lo_25", "Qst_Bt_Lo_00_Bt_Lo_26",
        "Qst_Bt_Lo_00_Bt_Lo_27", "Qst_Bt_Lo_00_Bt_Lo_28", "Qst_Bt_Lo_00_Bt_Lo_29",
        "Qst_Bt_Lo_00_Bt_Lo_30");
    bss_00246d58[5] = new (data_002232d5) NameTable(12, 6, "Qst_Bg_Up_06", 0);
    bss_00246d58[6] = new (data_002232d5) NameTable();
    bss_00246d74[0] = new (data_002232d5) StartupLink(12, 98);
    bss_00246d74[1] = new (data_002232d5) StartupLink(12, 99);
    bss_00246d74[2] = new (data_002232d5) StartupLink(12, 100);
    bss_00246d74[3] = new (data_002232d5) StartupLink(12, 101);
    bss_00246d74[4] = new (data_002232d5) StartupLink(12, 102);
    bss_00246d74[5] = new (data_002232d5) StartupLink(12, 103);
    bss_00246d74[6] = new (data_002232d5) StartupLink(12, 104);
    bss_00246d74[7] = new (data_002232d5) StartupLink(12, 105);
    bss_00246d74[8] = new (data_002232d5) StartupLink(12, 106);
    bss_00246d74[9] = new (data_002232d5) StartupLink(12, 107);
    bss_00246d74[10] = new (data_002232d5) StartupLink(12, 108);
    bss_00246d74[11] = new (data_002232d5) StartupLink(12, 109);
    bss_00246d74[12] = new (data_002232d5) StartupLink(12, 110);
    bss_00246d74[13] = new (data_002232d5) StartupLink(12, 111);
    bss_00246d74[14] = new (data_002232d5) StartupLink(12, 112);
    bss_00246d74[15] = new (data_002232d5) StartupLink(12, 113);
    bss_00246d74[16] = new (data_002232d5) StartupLink(12, 114);
    bss_00246db8[0] = new (data_002232d5) StartupLink(12, 115);
    bss_00246db8[1] = new (data_002232d5) StartupLink(12, 116);
    bss_00246db8[2] = new (data_002232d5) StartupLink(12, 117);
    bss_00246db8[3] = new (data_002232d5) StartupLink(12, 118);
    bss_00246db8[4] = new (data_002232d5) StartupLink(12, 119);
    bss_00246db8[5] = new (data_002232d5) StartupLink(12, 120);
    bss_00246db8[6] = new (data_002232d5) StartupLink(12, 121);
    bss_00246db8[7] = new (data_002232d5) StartupLink(12, 122);
    bss_00246db8[8] = new (data_002232d5) StartupLink(12, 123);
    bss_00246db8[9] = new (data_002232d5) StartupLink(12, 124);
    bss_00246db8[10] = new (data_002232d5) StartupLink(12, 125);
    bss_00246db8[11] = new (data_002232d5) StartupLink(12, 126);
    bss_00246db8[12] = new (data_002232d5) StartupLink(12, 127);
    bss_00246db8[13] = new (data_002232d5) StartupLink(12, 128);
    bss_00246db8[14] = new (data_002232d5) StartupLink(12, 129);
    bss_00246db8[15] = new (data_002232d5) StartupLink(12, 130);
    bss_00246db8[16] = new (data_002232d5) StartupLink(12, 131);
    bss_00246dfc[0] = new (data_002232d5) StartupLink(12, 132);
    bss_00246dfc[1] = new (data_002232d5) StartupLink(12, 133);
    bss_00246dfc[2] = new (data_002232d5) StartupLink(12, 134);
    bss_00246dfc[3] = new (data_002232d5) StartupLink(12, 135);
    bss_00246dfc[4] = new (data_002232d5) StartupLink(12, 136);
    bss_00246dfc[5] = new (data_002232d5) StartupLink(12, 137);
    bss_00246dfc[6] = new (data_002232d5) StartupLink(12, 138);
    bss_00246dfc[7] = new (data_002232d5) StartupLink(12, 139);
    bss_00246dfc[8] = new (data_002232d5) StartupLink(12, 140);
    bss_00246dfc[9] = new (data_002232d5) StartupLink(12, 141);
    bss_00246dfc[10] = new (data_002232d5) StartupLink(12, 142);
    bss_00246dfc[11] = new (data_002232d5) StartupLink(12, 143);
    bss_00246e2c[0] = new (data_002232d5) StartupLink(12, 144);
    bss_00246e2c[1] = new (data_002232d5) StartupLink(12, 145);
    bss_00246e2c[2] = new (data_002232d5) StartupLink(12, 146);
    bss_00246e2c[3] = new (data_002232d5) StartupLink(12, 147);
    bss_00246e2c[4] = new (data_002232d5) StartupLink(12, 148);
    bss_00246e2c[5] = new (data_002232d5) StartupLink(12, 149);
    bss_00246e2c[6] = new (data_002232d5) StartupLink(12, 150);
    bss_00246e2c[7] = new (data_002232d5) StartupLink(12, 151);
    bss_00246e2c[8] = new (data_002232d5) StartupLink(12, 152);
    bss_00246e2c[9] = new (data_002232d5) StartupLink(12, 153);
    bss_00246e2c[10] = new (data_002232d5) StartupLink(12, 154);
    bss_00246e2c[11] = new (data_002232d5) StartupLink(12, 155);
    bss_00246e2c[12] = new (data_002232d5) StartupLink(12, 156);
    bss_00246e2c[13] = new (data_002232d5) StartupLink(12, 157);
    bss_00246e2c[14] = new (data_002232d5) StartupLink(12, 158);
    bss_00246e2c[15] = new (data_002232d5) StartupLink(12, 159);
    bss_00246e2c[16] = new (data_002232d5) StartupLink(12, 160);
    bss_00246e2c[17] = new (data_002232d5) StartupLink(12, 161);
    bss_00246e2c[18] = new (data_002232d5) StartupLink(12, 162);
    bss_00246e2c[19] = new (data_002232d5) StartupLink(12, 163);
    bss_00246e2c[20] = new (data_002232d5) StartupLink(12, 164);
    bss_00246e2c[21] = new (data_002232d5) StartupLink(12, 165);
    bss_00246e2c[22] = new (data_002232d5) StartupLink(12, 166);
    bss_00246e2c[23] = new (data_002232d5) StartupLink(12, 167);
    bss_00246e2c[24] = new (data_002232d5) StartupLink(12, 168);
    bss_00246e2c[25] = new (data_002232d5) StartupLink(12, 169);
    bss_00246e2c[26] = new (data_002232d5) StartupLink(12, 170);
    bss_00246e2c[27] = new (data_002232d5) StartupLink(12, 171);
    bss_00246e2c[28] = new (data_002232d5) StartupLink(12, 175);
    bss_00246e2c[29] = new (data_002232d5) StartupLink(12, 176);
    bss_00246e2c[30] = new (data_002232d5) StartupLink(12, 172);
    bss_00246e2c[31] = new (data_002232d5) StartupLink(12, 173);
    bss_00246e2c[32] = new (data_002232d5) StartupLink(12, 174);
    data_0023ecf4 = new (data_002232d5) StartupItemPageSet();
}
