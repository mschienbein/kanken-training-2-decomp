#include "game/sound_task.h"
#include "game/object_manager.h"
#include "game/service_state.h"

struct StreamMutex bss_0029f528;
struct StreamMutex bss_0029f534;
extern void func_0010ce58(int (*callback)(void *context), void *context);
extern void func_0010ce44(void (*callback)(void *context), void *context);
extern void func_0010cf84(void (*callback)(void *context), void *context);

void func_00106e04(void)
{
    bss_0029f528.depth = -1;
    bss_0029f534.depth = -1;
    func_0010ce58(0, 0);
    func_0010ce44(0, 0);
    func_0010cf84(0, 0);
}
