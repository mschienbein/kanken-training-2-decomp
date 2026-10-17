#include "game/glyph_style.h"
#include "game/render_parameter.h"
#include "game/runtime.h"
#include <stddef.h>

/* [INFERENCE] This 16-byte text style stores an enable flag, two scale
 * factors, and an RGBA color. Class/member names are descriptive. */



void func_00204454(void)
{
    bss_00291244[0] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 231, 29, 29, 255);
    bss_00291244[1] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 79, 79, 79, 255);
    bss_00291244[2] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 79, 79, 79, 255);
    bss_00291244[3] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 79, 79, 79, 255);
    bss_00291244[4] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 79, 79, 79, 255);
    bss_00291244[5] = new (data_002232d5) RenderParameter(0.75f, 0.75f, 65, 127, 43, 255);
    bss_00291244[6] = new (data_002232d5) RenderParameter(0.85f, 0.85f, 254, 231, 161, 255);
    bss_00291244[7] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 65, 127, 43, 255);
    bss_00291244[8] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 231, 68, 66, 255);
    bss_00291244[9] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 231, 68, 66, 255);
    bss_00291244[10] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 79, 79, 79, 255);
    bss_00291244[11] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 42, 95, 125, 255);
    bss_00291244[12] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[13] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[14] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[15] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[16] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[17] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[18] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[19] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[20] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[21] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[22] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[23] = new (data_002232d5) RenderParameter(0.45f, 0.45f, 161, 104, 38, 255);
    bss_00291244[24] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 161, 104, 38, 255);
    bss_00291244[25] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 161, 104, 38, 255);
    bss_00291244[26] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 161, 104, 38, 255);
    bss_00291244[27] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 161, 104, 38, 255);
    bss_00291244[28] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 161, 104, 38, 255);
    bss_00291244[29] = new (data_002232d5) RenderParameter(1.0f, 1.0f, 161, 104, 38, 255);
    bss_00291244[30] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[31] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[32] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[33] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[34] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[35] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[36] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[37] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[38] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[39] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[40] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[41] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[42] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[43] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[44] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[45] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[46] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[47] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[48] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[49] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[50] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[51] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[52] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[53] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[54] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[55] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[56] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[57] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[58] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[59] = new (data_002232d5) RenderParameter(0.55f, 0.55f, 79, 79, 79, 255);
    bss_00291244[60] = new (data_002232d5) RenderParameter(0.6f, 0.6f, 79, 79, 79, 255);
    bss_00291244[61] = new (data_002232d5) RenderParameter(0.6f, 0.6f, 79, 79, 79, 255);
    bss_00291244[62] = new (data_002232d5) RenderParameter(0.6f, 0.6f, 79, 79, 79, 255);
    bss_00291244[63] = new (data_002232d5) RenderParameter();
}
