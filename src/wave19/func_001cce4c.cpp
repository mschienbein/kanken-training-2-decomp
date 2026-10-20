#include "game/pool_list.h"
#include "game/layout_binding.h"



extern "C" LayoutBinding *func_001cce4c(LayoutBinding *binding)
{
    binding->link.next = 0;
    binding->link.previous = 0;
    binding->count = 0;
    binding->frame = 0.0f;
    binding->first_allocation = 0;
    binding->vtable = data_00224100;
    binding->second_allocation = 0;
    binding->flags = 0;
    return binding;
}
