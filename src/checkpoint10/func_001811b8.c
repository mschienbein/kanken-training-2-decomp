#include "game/service_state.h"
#include "ctr/svc.h"

/* Activity state polled by the cooperative idle wait: +0 selects between the
 * counter and the service poll, +8 counts outstanding work. [INFERENCE] names. */
struct RuntimeActivity {
    unsigned int initialized;
    unsigned int reserved04;
    unsigned int outstanding;
};


extern int func_001900ac(void);

/* Qualification: every svc 0x0a in the image (0x104c04, 0x110f5c, 0x1811e0,
 * 0x18120c) is followed by exactly two NOP hint words before the loop's back
 * branch. The compiler emits no NOP after svcSleepThread by itself, so the two
 * words are written as the documented __nop() intrinsic. [INFERENCE] The
 * original sleep helper that owns them is not recovered. */
void func_001811b8(void)
{
    if (bss_004990d0.initialized != 0) {
        for (;;) {
            if (bss_004990d0.outstanding == 0)
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
