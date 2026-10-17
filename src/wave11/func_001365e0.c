#include "game/service_protocol.h"
unsigned func_001365e0(const struct ServiceConfiguration *configuration)
{
    unsigned result = func_001426f4(configuration->label);
    switch (result >> 31) {
    case 0:
        return 0;
    default:
        return result;
    }
}
