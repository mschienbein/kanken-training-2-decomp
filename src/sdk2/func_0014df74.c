#include "game/input_runtime.h"

extern void func_0014dfa8(struct InputController *controller, int index,
                          const struct ExtendedInputUpdate *update);
void func_0014df74(struct InputSnapshot *snapshot, signed char index,
                   const struct ExtendedInputUpdate *update)
{
    snapshot->primary[1][index] = update->value != 0;
    func_0014dfa8(func_00185044(), index, update);
}
