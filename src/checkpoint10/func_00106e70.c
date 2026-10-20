#include <stdbool.h>

struct W23JActivityState {
    bool active;
    bool latched;
    unsigned char reserved02[0x16];
    long long interval;
};
extern struct W23JActivityState data_00242c18;
extern int func_0010c0a4(void);
extern int func_0010bff4(void);

void func_00106e70(void)
{
    data_00242c18.interval = (long long)(10 * func_0010c0a4());
    if (func_0010bff4())
        data_00242c18.latched = 1;
    if (data_00242c18.latched || data_00242c18.interval != 0)
        data_00242c18.active = 1;
}
