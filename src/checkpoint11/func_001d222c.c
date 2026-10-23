#include "game/linked_owners.h"

/* Catalogue v8 separates this callback from its retained unlink destination. */
void func_001d222c(struct LinkedView *view)
{
    func_001d2234(view->linked);
}
