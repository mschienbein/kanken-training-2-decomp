#include "game/language.h"
#include "game/result.h"
#include "game/runtime.h"
#include "ctr/ipc.h"
#include "game/service_protocol.h"

/* [INFERENCE] Region/language roles and static helper boundaries follow the
 * supported-language masks and fallback values; original TU identity is unknown. */
namespace {

static void check_result(unsigned address, Result result)
{
    switch (result.failed()) {
    case 0: break;
    default: func_0019d6c4(result.value, address); break;
    }
}

static bool language_in_mask(unsigned language, unsigned supported)
{
    /* ORIGINAL BUG: the configured language is assumed to be below 32. */
    return ((1u << language) & supported) != 0;
}

static unsigned default_language(unsigned region)
{
    switch (region) {
    default: return 0;
    case 1:
    case 2: return 1;
    case 4: return 6;
    case 5: return 7;
    case 6: return 11;
    }
}

/* [INFERENCE] A source-level grouping of the two regional language sets,
 * not an independently recovered historical class or helper boundary. */
struct RegionalMasks {
    unsigned america;
    unsigned europe;
};

static bool language_is_supported(const RegionalMasks &masks, unsigned region, unsigned language)
{
    bool supported;
    switch (region) {
    case 0: supported = language_in_mask(language, 1); break;
    case 1: supported = language_in_mask(language, masks.america); break;
    case 2: supported = language_in_mask(language, masks.europe); break;
    case 4: supported = language_in_mask(language, 0x40); break;
    case 5: supported = language_in_mask(language, 0x80); break;
    case 6: supported = language_in_mask(language, 0x800); break;
    default: supported = 1; break;
    }
    return supported;
}

} // namespace

extern "C" unsigned func_0012e68c(void)
{
    unsigned char configuration = data_0021c5d8;
    Result result = func_00121764(&configuration, 1, 0xa0002);
    unsigned address = __current_pc();
    check_result(address, result);
    unsigned language = configuration;
    unsigned region = func_0013685c();
    RegionalMasks masks;
    masks.america = 0x226;
    masks.europe = 0x73e;
    return language_is_supported(masks, region, language) ? language : default_language(region);
}
