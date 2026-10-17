struct W23JDrawRange {
    unsigned offset;
    unsigned size;
    unsigned first;
    unsigned count;
};
extern void func_0011e358(unsigned handle, unsigned offset, unsigned size,
                         unsigned first, unsigned count, unsigned flags,
                         int immediate);

void func_001966bc(unsigned handle, const struct W23JDrawRange *range)
{
    func_0011e358(handle, range->offset, range->size, range->first,
                  range->count, 0, 1);
}
