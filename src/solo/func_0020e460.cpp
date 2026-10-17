#include "game/resource_descriptors.h"
#include "game/runtime.h"
#include <stddef.h>

/* [INFERENCE] Class and member names describe the recovered layouts; the
 * original declarations and their semantic names have not been recovered. */




void func_0020e460(void)
{
    bss_00246748[0] = new (data_002232d5) LayoutDescriptor(
        3, 10, 9, -1, -1, -1, 55.0f, 55.0f, 55.0f, 0.0f, 0.0f, 0.0f);
    {
        LayoutDescriptor *layout = static_cast<LayoutDescriptor *>(
            func_0019ffa4(sizeof(LayoutDescriptor), data_002232d5));
        if (layout != 0)
        {
            float trailing_offset = 0.0f;
            float compact_offset = 23.0f;
            layout = func_00164b14(layout, 4, 11, 9, 9, 9, -1,
                24.0f, 24.0f, 65.0f, compact_offset, -19.0f, trailing_offset);
        }
        bss_00246748[1] = layout;
    }
    bss_00246748[2] = new (data_002232d5) LayoutDescriptor(
        3, 12, 9, -1, -1, -1, 55.0f, 55.0f, 55.0f, 0.0f, 0.0f, 0.0f);
    bss_00246748[3] = new (data_002232d5) LayoutDescriptor(
        3, 13, 9, 9, -1, -1, 55.0f, 55.0f, 74.0f, 34.0f, 0.0f, 0.0f);
    bss_00246748[4] = new (data_002232d5) LayoutDescriptor(
        3, 14, 9, 9, -1, -1, 55.0f, 55.0f, 74.0f, 34.0f, 0.0f, 0.0f);
    bss_00246748[5] = new (data_002232d5) LayoutDescriptor(
        3, 15, -1, -1, -1, -1, 55.0f, 55.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    bss_00246748[6] = new (data_002232d5) LayoutDescriptor(
        3, 16, 9, 9, -1, -1, 23.0f, 23.0f, 2.0f, 42.0f, 0.0f, 0.0f);
    bss_00246748[7] = new (data_002232d5) LayoutDescriptor(
        0, 17, -1, -1, -1, -1, 23.0f, 23.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    bss_00246748[8] = new (data_002232d5) LayoutDescriptor(
        4, 18, -1, -1, -1, -1, 24.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    bss_00246748[9] = new (data_002232d5) LayoutDescriptor(
        6, 19, 3, -1, -1, -1, 87.0f, -38.0f, -6.0f, 0.0f, 0.0f, 0.0f);
    bss_00246748[10] = new (data_002232d5) LayoutDescriptor(
        6, 19, 7, 3, -1, -1, 87.0f, -38.0f, 105.0f, -6.0f, 0.0f, 0.0f);
    bss_00246748[11] = new (data_002232d5) LayoutDescriptor(
        6, 19, 8, 3, -1, -1, 87.0f, -38.0f, 105.0f, -6.0f, 0.0f, 0.0f);
    bss_00246748[12] = new (data_002232d5) LayoutDescriptor(
        5, 20, 9, 9, 9, -1, 24.0f, 24.0f, 76.0f, 41.0f, 6.0f, 0.0f);
    bss_00246748[13] = new (data_002232d5) LayoutDescriptor(
        4, 21, 9, 9, 9, -1, 24.0f, 24.0f, 56.0f, 22.0f, -12.0f, 0.0f);
    bss_00246748[14] = new (data_002232d5) LayoutDescriptor(
        3, 22, 9, -1, -1, -1, 21.0f, 21.0f, 20.0f, 0.0f, 0.0f, 0.0f);
    bss_00246748[15] = new (data_002232d5) LayoutDescriptor(
        5, 23, 9, 9, 9, -1, 24.0f, 24.0f, 76.0f, 41.0f, 6.0f, 0.0f);
    bss_00246748[16] = new (data_002232d5) LayoutDescriptor(
        3, 28, -1, -1, -1, -1, 55.0f, 55.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    bss_00246748[17] = new (data_002232d5) LayoutDescriptor(
        0, 29, -1, -1, -1, -1, 55.0f, 55.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    bss_00246790[0] = new (data_002232d5) ResourceDescriptor(1, 21, "Cmn_Win_Lo_B_00");
    bss_00246790[1] = new (data_002232d5) ResourceDescriptor(1, 21, "Cmn_Win_Lo_B_00");
    bss_00246790[2] = new (data_002232d5) ResourceDescriptor(1, 21, "Cmn_Win_Lo_B_00");
    bss_00246790[3] = new (data_002232d5) ResourceDescriptor(1, 23, "Cmn_Win_Lo_B_01");
    bss_00246790[4] = new (data_002232d5) ResourceDescriptor(1, 24, "Cmn_Win_Lo_B_02");
    bss_00246790[5] = new (data_002232d5) ResourceDescriptor(1, 25, "Cmn_Win_Lo_B_03");
    bss_00246790[6] = new (data_002232d5) ResourceDescriptor(8, 39, "Rec_Yam_Win_00");
    bss_00246790[7] = new (data_002232d5) ResourceDescriptor(8, 41, "Rec_Yam_Fla_00");
    bss_00246790[8] = new (data_002232d5) ResourceDescriptor(8, 42, "Rec_Yam_Fla_01");
    bss_00246790[9] = new (data_002232d5) ResourceDescriptor(1, 27, "Cmn_Win_Lo_Line_00");
    bss_00246790[10] = new (data_002232d5) ResourceDescriptor(5, 26, "Knt_Win_Lbl_Lo_00");
    bss_00246790[11] = new (data_002232d5) ResourceDescriptor(5, 27, "Knt_Win_Lbl_Lo_01");
    bss_00246790[12] = new (data_002232d5) ResourceDescriptor(6, 38, "Krn_Win_Lbl_Lo_00");
    bss_00246790[13] = new (data_002232d5) ResourceDescriptor(6, 39, "Krn_Win_Lbl_Lo_01");
    bss_00246790[14] = new (data_002232d5) ResourceDescriptor(7, 5, "Myk_Win_Lbl_Lo_00");
    bss_00246790[15] = new (data_002232d5) ResourceDescriptor(10, 3, "Abi_Win_Lbl_Lo_00");
    bss_00246790[16] = new (data_002232d5) ResourceDescriptor(10, 4, "Abi_Win_Lbl_Lo_01");
    bss_00246790[17] = new (data_002232d5) ResourceDescriptor(10, 5, "Abi_Win_Lbl_Lo_02");
    bss_00246790[18] = new (data_002232d5) ResourceDescriptor(8, 47, "Rec_His_No_00");
    bss_00246790[19] = new (data_002232d5) ResourceDescriptor(8, 15, "Rec_Win_Lbl_Lo_05");
    bss_00246790[20] = new (data_002232d5) ResourceDescriptor(11, 5, "Fnc_Win_Lbl_Lo_00");
    bss_00246790[21] = new (data_002232d5) ResourceDescriptor(11, 6, "Fnc_Win_Lbl_Lo_01");
    bss_00246790[22] = new (data_002232d5) ResourceDescriptor(11, 7, "Fnc_Win_Lbl_Lo_03");
    bss_00246790[23] = new (data_002232d5) ResourceDescriptor(11, 8, "Fnc_Win_Lbl_Lo_04");
    bss_00246790[24] = new (data_002232d5) ResourceDescriptor(3, 30, "Usr_Win_Lbl_Lo_00");
    bss_00246790[25] = new (data_002232d5) ResourceDescriptor(3, 31, "Usr_Win_Lbl_Lo_01");
    bss_00246790[26] = new (data_002232d5) ResourceDescriptor(3, 32, "Usr_Win_Lbl_Lo_02");
    bss_00246790[27] = new (data_002232d5) ResourceDescriptor(3, 33, "Usr_Win_Lbl_Lo_03");
    bss_00246790[28] = new (data_002232d5) ResourceDescriptor(9, 30, "Rnk_Win_Lbl_Lo_00");
    bss_00246790[29] = new (data_002232d5) ResourceDescriptor(9, 31, "Rnk_Win_Lbl_Lo_01");
    bss_00246790[30] = new (data_002232d5) ResourceDescriptor();
    bss_0024680c[0] = new (data_002232d5) ResourceDescriptor(1, 21, "Cmn_Win_Lo_B_00");
    bss_0024680c[1] = new (data_002232d5) ResourceDescriptor(1, 21, "Cmn_Win_Lo_B_00");
    bss_0024680c[2] = new (data_002232d5) ResourceDescriptor(1, 21, "Cmn_Win_Lo_B_00");
    bss_0024680c[3] = new (data_002232d5) ResourceDescriptor(1, 23, "Cmn_Win_Lo_B_01");
    bss_0024680c[4] = new (data_002232d5) ResourceDescriptor(1, 24, "Cmn_Win_Lo_B_02");
    bss_0024680c[5] = new (data_002232d5) ResourceDescriptor(1, 25, "Cmn_Win_Lo_B_03");
    bss_0024680c[6] = new (data_002232d5) ResourceDescriptor(8, 39, "Rec_Yam_Win_00");
    bss_0024680c[7] = new (data_002232d5) ResourceDescriptor(8, 41, "Rec_Yam_Fla_00");
    bss_0024680c[8] = new (data_002232d5) ResourceDescriptor(8, 42, "Rec_Yam_Fla_01");
    bss_0024680c[9] = new (data_002232d5) ResourceDescriptor(1, 27, "Cmn_Win_Lo_Line_00");
    bss_0024680c[10] = new (data_002232d5) ResourceDescriptor(5, 67, "Knt_Win_Lbl_Lo_00_h");
    bss_0024680c[11] = new (data_002232d5) ResourceDescriptor(5, 68, "Knt_Win_Lbl_Lo_01_h");
    bss_0024680c[12] = new (data_002232d5) ResourceDescriptor(6, 86, "Krn_Win_Lbl_Lo_00_h");
    bss_0024680c[13] = new (data_002232d5) ResourceDescriptor(6, 87, "Krn_Win_Lbl_Lo_01_h");
    bss_0024680c[14] = new (data_002232d5) ResourceDescriptor(7, 45, "Myk_Win_Lbl_Lo_00_h");
    bss_0024680c[15] = new (data_002232d5) ResourceDescriptor(10, 3, "Abi_Win_Lbl_Lo_00");
    bss_0024680c[16] = new (data_002232d5) ResourceDescriptor(10, 49, "Abi_Win_Lbl_Lo_01_h");
    bss_0024680c[17] = new (data_002232d5) ResourceDescriptor(10, 5, "Abi_Win_Lbl_Lo_02");
    bss_0024680c[18] = new (data_002232d5) ResourceDescriptor(8, 47, "Rec_His_No_00");
    bss_0024680c[19] = new (data_002232d5) ResourceDescriptor(8, 15, "Rec_Win_Lbl_Lo_05");
    bss_0024680c[20] = new (data_002232d5) ResourceDescriptor(11, 15, "Fnc_Win_Lbl_Lo_00_h");
    bss_0024680c[21] = new (data_002232d5) ResourceDescriptor(11, 16, "Fnc_Win_Lbl_Lo_01_h");
    bss_0024680c[22] = new (data_002232d5) ResourceDescriptor(11, 17, "Fnc_Win_Lbl_Lo_03_h");
    bss_0024680c[23] = new (data_002232d5) ResourceDescriptor(11, 18, "Fnc_Win_Lbl_Lo_04_h");
    bss_0024680c[24] = new (data_002232d5) ResourceDescriptor(3, 30, "Usr_Win_Lbl_Lo_00");
    bss_0024680c[25] = new (data_002232d5) ResourceDescriptor(3, 31, "Usr_Win_Lbl_Lo_01");
    bss_0024680c[26] = new (data_002232d5) ResourceDescriptor(3, 32, "Usr_Win_Lbl_Lo_02");
    bss_0024680c[27] = new (data_002232d5) ResourceDescriptor(3, 33, "Usr_Win_Lbl_Lo_03");
    bss_0024680c[28] = new (data_002232d5) ResourceDescriptor(9, 30, "Rnk_Win_Lbl_Lo_00");
    bss_0024680c[29] = new (data_002232d5) ResourceDescriptor(9, 31, "Rnk_Win_Lbl_Lo_01");
    bss_0024680c[30] = new (data_002232d5) ResourceDescriptor();
}
