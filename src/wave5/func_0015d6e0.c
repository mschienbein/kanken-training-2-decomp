struct NotificationState {
    unsigned char reserved00[2];
    unsigned char pending;
    unsigned char value;
    unsigned char enabled;
    unsigned char acknowledged;
};
extern struct NotificationState bss_00290cac;

void func_0015d6e0(int value)
{
    bss_00290cac.pending = 1;
    bss_00290cac.value = value;
    bss_00290cac.enabled = 1;
    bss_00290cac.acknowledged = 0;
}
