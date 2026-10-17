struct ResourceSuspendFlags {
    unsigned char reserved00[3];
    unsigned char suspended;
};

extern struct ResourceSuspendFlags data_0023ee9c;

void func_001864cc(unsigned char suspended)
{
    data_0023ee9c.suspended = suspended;
}
