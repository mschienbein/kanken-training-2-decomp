/* [INFERENCE] Descriptive type, matching the prior wrapper candidate
 * scratch/authors/w18e/src/func_001d222c.c. The wrapper selects linked;
 * this body clears both sides of that relationship. */
#include "game/linked_owners.h"

void func_001d2234(struct LinkedView *view)
{
    if (view->peer) {
        if (view->peer->linked == view)
            view->peer->linked = 0;
        if (view->peer)
            view->peer = 0;
    }
}
