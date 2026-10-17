struct RestorableState {
    unsigned current;
    unsigned saved;
};
extern struct RestorableState data_0023fdc8;

void func_0013dea0(void)
{
    data_0023fdc8.current = data_0023fdc8.saved;
}
