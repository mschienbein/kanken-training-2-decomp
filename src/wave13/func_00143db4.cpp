#include "game/locked_list.h"
#include "game/object_manager.h"
#include "game/pool_list.h"





LockedListOwner::LockedListOwner()
    : vtable(&data_00224ae8), lock(), lists()
{
    func_00195ec4(&lock);
}
