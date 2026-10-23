#include "game/service_result.h"


static bool result_has_module(unsigned value, unsigned module)
{
    return (value & 0x3fc00) == module;
}

static bool result_description_in_range(unsigned value, unsigned first, unsigned count)
{
    return (value & 0x3ff) - first < count;
}

int func_001266e4(const ServiceResult *result)
{
    unsigned value = func_0019e81c(result).value;
    if (!result_has_module(value, 0x13800))
        return 0;
    return result_description_in_range(value, 0xc0, 0x40)
        || result_description_in_range(value, 0x140, 0x40)
        || result_description_in_range(value, 0x40, 0x20);
}

int func_0012672c(const ServiceResult *result)
{
    int value = (int)func_0019e81c(result).value;
    int description;
    if (!result_has_module(value, 0x13800))
        goto other;
    switch (value) {
    case (int)0xc8a13909u:
    case (int)0xc8c13885u:
    case (int)0xc941390du:
    case (int)0xc9413910u:
    case (int)0xc9413912u:
    case (int)0xc961391au:
        goto recognized;
    }
    description = value & 0x3ff;
    if (description < 0x60)
        goto other;
    if (description >= 0x80)
        goto other;
recognized:
    return 1;
other:
    return 0;
}

