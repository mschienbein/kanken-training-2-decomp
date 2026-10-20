struct SparseResourceState {
    void *resource;
    unsigned reserved04;
    unsigned count;
};

extern struct SparseResourceState bss_004962a8;

void func_001043e0(void)
{
    bss_004962a8.resource = 0;
    bss_004962a8.count = 0;
}
