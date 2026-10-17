#include "game/functions.h"
#include "game/screen.h"

typedef void (*SceneCallback)(void);
/* Each of the 21 original table entries resolves to a catalogued function;
 * the scene dispatch wrappers and their callers discard callback results. */
static SceneCallback scene_entry_at(const SceneCallback *table, int index)
{
    return table[index];
}

/* Direct non-tail callers ignore the result; the index is a signed word.
 * [INFERENCE] The selection and entry type names. */
void func_00176dc8(int index)
{
    /* ORIGINAL BUG: a negative index passes the upper-bound-only check. */
    if (index < 21)
        data_0023ee9c.callback = scene_entry_at(data_0023ee9c.callbacks, index);
    data_0023ee9c.index = index;
    data_0023ee9c.pending = 0;
}
