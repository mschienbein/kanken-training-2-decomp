#include "game/image_activation.h"
#include "game/resource_tables.h"
#include "game/hwr_image.h"
#include "game/command_state.h"

void func_0018191c(struct W10DrawingInput *state)
{
    unsigned char *resource;
    if (state->notification_active == 1)
        return;
    func_001811b8();
    func_001494dc(state);
    resource = func_001971a0(1, 3);
    if (resource) {
        int result = func_0014a988(127, 127, (struct HwrImage *)resource);
        state->field89a = result;
        if (result == 0) {
            state->notification_active = 1;
            /* [INFERENCE] The signed 16-bit snapshot follows the original
             * default read; the canonical destination and consumer use its bits. */
            const short default_flags = (short)data_002181c0.masks.fallback;
            state->effect_mask = default_flags;
            func_0014a9d8((unsigned short)default_flags);
        }
    }
}
