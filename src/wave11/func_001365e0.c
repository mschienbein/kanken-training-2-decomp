struct ServiceConfiguration {
    unsigned reserved00[3];
    unsigned value;
    unsigned char kind, variant, mode, reserved13;
    char label[0x40];
    char metadata[0x44];
    unsigned char changed;
};
extern unsigned func_001426f4(const char *text);

unsigned func_001365e0(const struct ServiceConfiguration *configuration)
{
    unsigned result = func_001426f4(configuration->label);
    switch (result >> 31) {
    case 0:
        return 0;
    default:
        return result;
    }
}
