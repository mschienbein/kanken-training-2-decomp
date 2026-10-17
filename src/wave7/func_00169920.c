#include "game/selection.h"

/* [INFERENCE] The fourth argument is passed through by this wrapper but is
 * unused by the target; callers consistently supply an integer sentinel. */
extern int func_0016992c(int group, int index, int mask, int value);

int func_00169920(int group, int index, int value)
{
    return func_0016992c(group, index, 4, value);
}
