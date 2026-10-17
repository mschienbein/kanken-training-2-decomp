/* Only the allocator address is used here; its lock/layout is not recovered. */
struct LinearAllocator;
extern struct LinearAllocator bss_004b204c;
extern unsigned char data_00242d74;
extern void func_001927b8(struct LinearAllocator *allocator, void *memory, unsigned size);

void func_0010be94(void *memory, unsigned size)
{
    if (data_00242d74)
        return;
    data_00242d74 = 1;
    func_001927b8(&bss_004b204c, memory, size);
}
