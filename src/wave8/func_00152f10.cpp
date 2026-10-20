#include "game/scene_lifecycle.h"
#include "game/prefetch_stream.h"


extern "C" void func_00152f10(nw::snd::internal::driver::Voice *resource, int event,
                   void *context)
{
    /* S08 and callback-producers-r2: original 00150778/00150cf8 pass
     * their channel-array member (+0x228, stride 0x3c8) as the fifth input
     * to 0017e500. This is that actual constructed StreamChannel, not a
     * separate two-word callback-state owner. */
    StreamChannel *state = static_cast<StreamChannel *>(context);
    if (event == 0 || event == 1) {
        func_00183550(resource);
        state->channel = 0;
    } else if (event == 2 || event == 3) {
        state->channel = 0;
    }
}
