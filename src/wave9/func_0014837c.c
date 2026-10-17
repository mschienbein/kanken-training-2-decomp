#include "game/input_runtime.h"

void func_0014837c(struct InputLatchSnapshot *snapshot)
{
    int i, group;
    for (i = 0; i < 2; ++i) {
        for (group = 0; group < 2; ++group) {
            snapshot->current[group][i] = 0;
            snapshot->primary[group][i] = 0;
            snapshot->changed[group][i] = 0;
        }
    }
    func_001483c0(func_00185044());
}
