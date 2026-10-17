#include "game/input_runtime.h"

extern void func_0014df0c(struct InputController *controller, int index,
                          const struct ShortInputUpdate *update);
void func_0014ded8(struct InputSnapshot *snapshot, signed char index,
                   const struct ShortInputUpdate *update)
{
    snapshot->primary[0][index] = update->value != 0;
    func_0014df0c(func_00185044(), index, update);
}
