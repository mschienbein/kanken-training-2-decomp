struct MemorySpan {
    unsigned base;
    unsigned size;
};

extern struct MemorySpan data_00242d68;

unsigned func_0019d048(void)
{
    return data_00242d68.size;
}
