#include "ctr/ipc.h"
extern unsigned func_00192a7c(void);


int func_0012dcc0(unsigned *output)
{
    unsigned handle = func_00192a7c();
    return func_001365f8(&handle, output);
}
