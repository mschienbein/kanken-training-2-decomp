#include "game/render_state.h"

/* [INFERENCE] Surface descriptor identity; func_0011de68 dereferences
 * the returned pointer for dimensions, storage and pixel format. */
struct TransferSurface;


struct TransferSurface *func_00126f44(void)
{
    return data_002426d8->bound_framebuffer->attachments[0].surface;
}
