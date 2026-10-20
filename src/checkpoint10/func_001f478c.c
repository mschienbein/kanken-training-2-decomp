/* [INFERENCE] The signed count and node identity follow the prior wrapper
 * candidate scratch/authors/w19b/src/func_001f4784.c. The wrapper forwards
 * the address of its head field; this body reads but does not change it. */
#include "game/counted_list.h"

int func_001f478c(const struct CountedNode *const *head)
{
    const struct CountedNode *node;
    int count = 0;

    for (node = *head; node; node = node->next)
        ++count;
    return count;
}
