#include "game/sdk_init.h"
void func_001135e8(void)
{
    long long value;
    CtrResult result = func_0011ef2c(&value, data_00220b8c, 20);
    if ((int)((unsigned)result & 0x80000000) < 0)
        func_0019dfbc(result);
    data_00243798.second = (unsigned)value;
}
