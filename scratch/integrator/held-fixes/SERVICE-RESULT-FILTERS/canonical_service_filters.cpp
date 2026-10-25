#include "game/service_result.h"

extern "C" Result func_0019e81c(const ServiceResult *result);

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

static unsigned result_field(Result code, unsigned mask, unsigned shift)
{
    return (code.value & mask) >> shift;
}

static int module(Result code)
{
    return result_field(code, 0x3fc00, 10);
}

static int description(Result code)
{
    return result_field(code, 0x3ff, 0);
}

/* [INFERENCE] Complete translator decision graph retained from nf134/np170,
 * reconciled to the sole ServiceResult owner, without FileDevice context or a
 * ResourceResult shadow. Every emitted function is included in this probe. */
Result func_0019e81c(const ServiceResult *result)
{
    Result code((unsigned)result->code);
    if (!code.value)
        return code;
    Result selected((unsigned)result->code);
    switch (module(selected)) {
    case 0x28:
    case 0x2e:
        return code;
    case 0x34:
        if (module(code) == 0x34 && description(code) >= 100 && description(code) < 300) {
            switch (static_cast<int>(code.value)) {
            case (int)0xc860d069u: return 0xc8613914u;
            case (int)0xc860d06eu: return 0xc8613913u;
            case (int)0xc900d075u: return 0xc8a13905u;
            case (int)0xc920d073u: return 0xc941390au;
            case (int)0xc920d086u: return 0xc941390cu;
            case (int)0xc920d08cu: return 0xc941390eu;
            case (int)0xc920d08fu: return 0xc941390bu;
            case (int)0xc920d095u: return 0xc941390du;
            case (int)0xc920d0a4u: return 0xc941390fu;
            case (int)0xc920d0a7u: return 0xc9413910u;
            case (int)0xc920d0a8u: return 0xc9413940u;
            case (int)0xe0a0d0a9u: return 0xc94138c2u;
            case (int)0xe0c0d066u: return 0xe0c13897u;
            case (int)0xe0e0d068u: return 0xe0e1388fu;
            }
        }
        switch (static_cast<int>(code.value)) {
        case (int)0xc8a0d018u: return 0xc9413917u;
        case (int)0xd860d020u: return 0xd861388eu;
        case (int)0xd860d024u: return 0xd861389au;
        case (int)0xd880d023u: return 0xd8813896u;
        }
        return code;
    case 0x20:
        switch (static_cast<int>(code.value)) {
        case (int)0xc8a08035u: code = Result(0xc8613916u); break;
        }
        return code;
    case 0x11:
        if (module(code) == 0x11 && description(code) >= 0x104 && description(code) < 0x118)
            return 0xc8a13909u;
        return 0xc9413915u;
    default:
        return selected;
    }
}
