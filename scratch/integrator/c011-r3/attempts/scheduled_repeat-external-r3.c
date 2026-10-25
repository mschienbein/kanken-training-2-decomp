#include "game/startup.h"

/* [INFERENCE] This external C procedure occupies a real int(void) scheduler
 * slot. Its result is unspecified and MUST be discarded; no definition or
 * historical return meaning is asserted here. */
extern int func_00110cc8(void);

void func_00107f0c(void)
{
    func_0019f338(3, func_00110cc8, 3);
    bss_00495c8c.tasks[3].repeat = 0;
}
