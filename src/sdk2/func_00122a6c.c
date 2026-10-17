#include "ctr/svc.h"
#include "game/graphics_state.h"
#include "game/stream_manager.h"

struct X03GraphicsReset { unsigned reserved, value; };
extern struct StreamMutex bss_004b4f6c;
extern const struct X03GraphicsReset data_0021c600;
extern void func_0012f894(struct GraphicsContext *context);
void func_00122a6c(void)
{
    func_00192bd8(&bss_004b4f6c);
    if (data_002430f0.initialized) {
        func_0012f894(data_002430f0.context);
        svcCloseHandle(data_002430f0.handle);
        data_002430f0.mutex.handle = data_0021c600.value;
        data_002430f0.initialized = 0;
        func_00195d74(&bss_004b4f6c);
        return;
    }
    func_00195d74(&bss_004b4f6c);
}
