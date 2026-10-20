#include "game/pool_list.h"
#include "game/layout_binding.h"



extern "C" LayoutBinding *func_001cce84(LayoutBinding *binding)
{
    binding->vtable = data_00224100;
    if (binding->second_allocation)
        func_00191aec(binding->second_allocation);
    if (binding->first_allocation)
        func_00191aec(binding->first_allocation);
    return binding;
}
