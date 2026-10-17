#include "game/exp_heap.h"
#include "game/functions.h"
#include "game/startup.h"
typedef void (*TaskCallback)(void);

extern void func_00195b6c(int mode, TaskCallback begin, TaskCallback update,
                          TaskCallback alternate, TaskCallback end);

extern void func_00110eac(int count);


extern void func_0019f338(unsigned task, TaskCallback callback, unsigned priority);

void func_001070e8(void)
{
    func_00195b6c(0, 0, 0, 0, 0);
    func_00110ed8(1024);
    func_00110eac(1024);
    func_00110a58();
    func_0019f338(16, func_001109fc, 4096);
}
