struct W11jEnableControl {
    unsigned char reserved00[0x74];
    unsigned char disabled;
};

void func_00177b28(struct W11jEnableControl *control, int enabled)
{
    if (enabled)
        control->disabled = 0;
    else
        control->disabled = 1;
}
