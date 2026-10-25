#include "game/filesystem_worker.h"
#include "game/result.h"
#include "game/synchronization.h"

static inline void check_result(unsigned pc, Result result)
{
    switch (result.failed()) {
    case 0: break;
    default: func_0019d6c4(result.value, pc); break;
    }
}

void func_0010af8c(void *argument, void *context)
{
    SignalState *signal = static_cast<SignalState *>(argument);
    for (;;) {
        struct FilesystemWorkItem *item;
        int i;
        func_00195d94(signal);
        if (bss_002d2138.stop)
            return;
        if (!bss_002d2138.state)
            continue;
        item = static_cast<FilesystemWorkItem *>(bss_002d2138.allocation.pointer);
        func_0011247c();
        func_00112464();
        if (item->reformat) {
            Result result = func_00120e40(1, 1, 1);
            /* [INFERENCE] the sign test before check_result reproduces the
             * original's predicated tst/lsrs sequence. */
            if (result.value & 0x80000000)
                check_result(__current_pc(), result);
        }
        bss_002d2138.value = func_00110f7c(data_00242218, item->kind);
        for (i = 0; i < static_cast<int>(bss_002d2138.allocation.count); ++i, ++item) {
            if (item->active) {
                if (bss_002d2138.value == 0) {
                    if (item->kind == 0)
                        bss_002d2138.value = func_00111570(item);
                    else if (item->kind == 1)
                        bss_002d2138.value = func_0011122c(item);
                }
                if (item->allocation)
                    func_0019dc20(item->allocation);
                item->active = 0;
                item->allocation = 0;
            }
        }
        {
            const char *path = data_00242218;
            if ((func_001131f8(path) & 0x80000000) == 0)
                func_0011343c(path);
        }
        func_00112470();
        func_00112488();
        bss_002d2138.state = 0;
    }
}
