#include "game/runtime.h"
#include "game/graphics_address.h"
#include "game/functions.h"

/* [INFERENCE] A command key is one addressable unsigned word: the observed
 * normalizer reads exactly that word without modifying it. */
struct CommandKeyWord { unsigned int value; };

extern "C" unsigned int *func_001ce05c(struct BatchRenderer *renderer, unsigned int *output,
                           const struct TextureKeys *keys)
{
    CommandKeyWord first = { keys->first };
    CommandKeyWord second = { keys->second };
    CommandKeyWord third = { keys->third };
    CommandTemplateBank &bank = data_0024352c;
    if (renderer->mode != 2) {
        bank.value_d0 = first.value;
        bank.value_e8 = second.value;
        bank.value_f0 = third.value;
        func_00192720(output, &data_002435ec, 0x58);
        output += 0x58 / 4;
    } else if (func_0016b404(&data_002435ec.words[4]) != func_0016b404(&first.value) ||
               func_0016b404(&data_002435ec.words[10]) != func_0016b404(&second.value) ||
               func_0016b404(&data_002435ec.words[12]) != func_0016b404(&third.value)) {
        bank.value_d0 = first.value;
        bank.value_e8 = second.value;
        bank.value_f0 = third.value;
        *output++ = first.value;
        *output++ = 0x000f00db;
        *output++ = data_002435ec.words[10];
        *output++ = 0x000f00f3;
        *output++ = data_002435ec.words[12];
        *output++ = 0x000f00fd;
    }
    return output;
}
