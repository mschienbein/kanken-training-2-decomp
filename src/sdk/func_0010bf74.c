#include "ctr/cp15.h"

struct MainThreadRecord {
    unsigned int state;
    unsigned char *owner;
};

extern struct MainThreadRecord data_00242d78;

void func_0010bf74(void)
{
    data_00242d78.owner = ctrGetThreadLocalStorage();
}
