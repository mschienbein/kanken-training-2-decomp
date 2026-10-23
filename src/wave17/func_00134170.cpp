#include "game/resettable_manager.h"



void func_00134170(void *storage)
{
    ResettableManager *manager = static_cast<ResettableManager *>(storage);
    manager->vtable = data_00223bf0;
    if (manager->current)
        manager->current = 0;
    func_00183234(manager);
}
