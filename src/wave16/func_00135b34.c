struct ResetFlags {
    int first;
    int second;
};

extern struct ResetFlags data_0023fdc8;

void func_00135b34(void)
{
    data_0023fdc8.first = 1;
    data_0023fdc8.second = 1;
}
