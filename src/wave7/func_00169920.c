#include "game/selection.h"
#include "game/item_text.h"

/* [INFERENCE] The fourth argument of func_0016992c is passed through by this
 * wrapper but is unused by the target; callers consistently supply an integer
 * sentinel. */

int func_00169920(int group, int index, int value)
{
    return func_0016992c(group, index, 4, value);
}
