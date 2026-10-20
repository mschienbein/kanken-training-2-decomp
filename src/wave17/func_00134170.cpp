#include "game/resettable_manager.h"



ResettableManager *func_00134170(ResettableManager *manager)
{
    manager->vtable = data_00223bf0;
    if (manager->current)
        manager->current = 0;
    return static_cast<ResettableManager *>(func_00183234(manager));
}
