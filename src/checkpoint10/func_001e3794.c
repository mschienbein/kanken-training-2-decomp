struct HeapManager;
extern struct HeapManager bss_004952f0;
extern void func_001176e0(struct HeapManager *heap, unsigned int type,
                         unsigned int size, unsigned int flags, void *allocation);

/* [INFERENCE] The unused first input is the heap-interface receiver. */
void func_001e3794(struct HeapManager *receiver, void *allocation)
{
    func_001176e0(&bss_004952f0, 0x10000, 256, 0, allocation);
}
