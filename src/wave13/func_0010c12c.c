#include "game/command_group.h"
#include "game/input.h"
/* func_0010c138 consumes the service's offset-4 ring pointer, not InputReader.index.
 * [INFERENCE] Touch-service identity; keep its unrecovered layout opaque. */

struct TouchInputService *func_0010c12c(void)
{
    return &bss_004b1fb8;
}
