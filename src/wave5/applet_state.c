/* Partial applet state; the flag names describe their offsets until callers
 * establish their roles.  Shared declaration candidate: data_00242a50. */
struct AppletState {
    unsigned char flag_00;
    unsigned char flag_01;
    unsigned char reserved02[4];
    unsigned char flag_06;
    unsigned char reserved07[2];
    unsigned char flag_09;
    unsigned char flag_0a;
    unsigned char reserved0b;
    unsigned char flag_0c;
    unsigned char flag_0d;
    unsigned char reserved0e[10];
    volatile unsigned mode;
};
extern struct AppletState data_00242a50;

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
    data_00242a50.flag_09 = 1;
}

void func_0012369c(void)
{
    data_00242a50.flag_01 = 1;
}

unsigned func_00192794(void)
{
    return data_00242a50.mode & 7;
}
