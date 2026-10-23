#include "game/application_configuration.h"

static void check_result(unsigned address, Result result)
{
    switch (result.failed()) {
    case 0:
        break;
    default:
        func_0019d6c4(result.value, address);
        break;
    }
}

static Result read_block(unsigned block, void *destination, unsigned size)
{
    return func_00121764(destination, size, block);
}

int func_0010bff4(void)
{
    const KernelConfiguration *system =
        reinterpret_cast<const KernelConfiguration *>(0x1ff80000);
    if (system->environment & 1)
        return 0;
    ApplicationConfiguration configuration;
    unsigned char state;
    Result result = func_0011375c(&state);
    Result sign(result.value & 0x80000000u);
    if (static_cast<int>(sign.value) < 0) {
        unsigned address = __current_pc();
        check_result(address, result);
    }
    result = read_block(0x130000, &configuration, sizeof(configuration));
    unsigned address = __current_pc();
    check_result(address, result);
    func_001136a4(state);
    if (configuration.latched & 1)
        return 1;
    return 0;
}

int func_0010c0a4(void)
{
    const KernelConfiguration *system =
        reinterpret_cast<const KernelConfiguration *>(0x1ff80000);
    if (system->environment & 1)
        return 0;
    ApplicationConfiguration configuration;
    unsigned char state;
    Result result = func_0011375c(&state);
    Result sign(result.value & 0x80000000u);
    if (static_cast<int>(sign.value) < 0) {
        unsigned address = __current_pc();
        check_result(address, result);
    }
    result = read_block(0x130000, &configuration, sizeof(configuration));
    unsigned address = __current_pc();
    check_result(address, result);
    func_001136a4(state);
    return configuration.timeout;
}
