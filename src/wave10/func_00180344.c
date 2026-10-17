struct W10InputNotifications {
    unsigned char reserved00[2];
    unsigned char changed;
};
extern struct W10InputNotifications bss_00495c04;

void func_00180344(void)
{
    bss_00495c04.changed = 1;
}
