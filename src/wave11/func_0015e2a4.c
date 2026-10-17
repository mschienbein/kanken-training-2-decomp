struct DisplayQueueFlags {
    unsigned char reserved00[2];
    unsigned char enabled;
};

extern struct DisplayQueueFlags data_0023ee9c;

void func_0015e2a4(unsigned char enabled)
{
    data_0023ee9c.enabled = enabled;
}
