#include "game/exp_heap.h"
#include "game/functions.h"
#include "game/startup.h"
typedef void (*TaskCallback)(void);

extern void func_00195b6c(int mode, TaskCallback begin, TaskCallback update,
                          TaskCallback alternate, TaskCallback end);

extern void func_00110eac(int count);

extern void func_00135128(int count);
extern void func_00135f1c(int count);
/* [INFERENCE] The third option is caller-evidenced but unused by the callee. */
extern void func_00135c74(int count, int mode, unsigned option);
extern void func_00135c10(int count);




extern void func_0019f338(unsigned task, TaskCallback callback, unsigned priority);

void func_00107140(void)
{
    func_00195b6c(0, 0, 0, 0, 0);
    func_00110ed8(1024);
    func_00110eac(1024);
    func_00135268(1, 1024);
    func_00135128(256);
    func_00135f1c(8);
    func_00135c74(1024, 1, 1);
    func_00135c10(1024);
    func_00135bb0(1024);
    func_00135b4c(1024);
    func_00110a58();
    func_0019f338(16, func_00110a28, 4096);
}
