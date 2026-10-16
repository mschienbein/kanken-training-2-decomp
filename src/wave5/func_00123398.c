struct PendingState {
    unsigned char reserved00;
    unsigned char pending;
};
extern struct PendingState data_00242a28;
extern int func_0012f980(void);

int func_00123398(void)
{
    if (data_00242a28.pending == 0)
        return 0;
    data_00242a28.pending = 0;
    return func_0012f980();
}
