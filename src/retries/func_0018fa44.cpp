#include "game/service_list.h"
#include "game/service_state.h"



unsigned func_0018fa44(const struct ServiceContext *state, void **output,
                      unsigned int generation)
{
    unsigned result = state->validate_generation(generation);
    switch (result >> 31) {
    case 0:
        *output = &bss_004b47d0;
        result = 0;
        break;
    }
    return result;
}
