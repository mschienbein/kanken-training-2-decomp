struct OptionalValue {
    unsigned char reserved00[8];
    int value;
    unsigned reserved0c;
    unsigned state;
};
extern struct OptionalValue bss_002d2138;

int func_00145b20(void)
{
    return bss_002d2138.state != 1 ? bss_002d2138.value : -1;
}
