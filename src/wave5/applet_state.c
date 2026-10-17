#include "game/service_state.h"

void func_00191dac(unsigned char value)
{
    data_00242a50.flag_0a = value;
}

unsigned char func_00122af0(void)
{
    return data_00242a50.flag_0a;
}

void func_00122fe8(unsigned char value)
{
    data_00242a50.flag_0d = value;
}

void func_00192484(unsigned char value)
{
    data_00242a50.flag_0c = value;
}

void func_00122ff8(void)
{
    data_00242a50.flag = 1;
}

void func_0012369c(void)
{
    data_00242a50.active = 1;
}

unsigned func_00192794(void)
{
    return data_00242a50.state & 7;
}
