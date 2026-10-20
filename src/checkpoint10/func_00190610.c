/* [INFERENCE] The inputs are raw command words; the helper copies three words. */
#include "game/shader.h"

void func_00190610(int register_address, unsigned first,
                   unsigned second, unsigned third)
{
    unsigned values[3];
    values[0] = first;
    values[1] = second;
    values[2] = third;
    func_00127ddc(register_address, values, 3, 1);
}
