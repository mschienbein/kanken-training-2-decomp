#include "game/input_runtime.h"


int func_0014df0c(struct InputController *controller, int index,
                  const struct ShortInputUpdate *update)
{
    struct ShortInputUpdate *configuration = &controller->primary[index];
    if ((update->flags & 1) != 0)
        configuration->value = update->value;
    if ((update->flags & 4) != 0) {
        configuration->mode = update->mode;
        configuration->parameters[0] = update->parameters[0];
        configuration->parameters[1] = update->parameters[1];
        configuration->parameters[2] = update->parameters[2];
        configuration->parameters[3] = update->parameters[3];
    }
    if ((update->flags & 2) != 0)
        configuration->data = update->data;
    return func_00180fe0(&bss_004b5284, index, update);
}
