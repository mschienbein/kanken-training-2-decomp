#include "game/service_state.h"
#include "ctr/svc.h"

/* The canonical WorkQueue's enabled word at +0 selects the idle-wait path;
 * active at +8 counts outstanding work. No separate activity object is used. */


extern int func_001900ac(void);

/* Qualification: every svc 0x0a in the image (0x104c04, 0x110f5c, 0x1811e0,
 * 0x18120c) is followed by exactly two NOP hint words before the loop's back
 * branch. The compiler emits no NOP after svcSleepThread by itself, so the two
 * words are written as the documented __nop() intrinsic. [INFERENCE] The
 * original sleep helper that owns them is not recovered. */
void func_001811b8(void)
{
    if (bss_004990d0.enabled != 0) {
        for (;;) {
            if (bss_004990d0.active == 0)
                break;
            {
                unsigned int low = 0;
                int high = 0;
                svcSleepThread(low, high);
            }
            __nop();
            __nop();
        }
    } else {
        for (;;) {
            if (!func_001900ac())
                break;
            {
                unsigned int low = 0;
                int high = 0;
                svcSleepThread(low, high);
            }
            __nop();
            __nop();
        }
    }
}
