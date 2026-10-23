#include "game/linked_owners.h"

/* Catalogue v8 separates this callback from its retained detach destination. */
void func_001d033c(struct LinkedOwner *object)
{
    func_001d0348(object->link);
}
