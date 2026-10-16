typedef struct BufferState {
    void *buffer;
    unsigned int count;
    unsigned int capacity;
} BufferState;

extern BufferState bss_0049d93c;
extern void func_0019dc20(void *memory);

void func_00135a88(void)
{
    if (bss_0049d93c.buffer) {
        func_0019dc20(bss_0049d93c.buffer);
        bss_0049d93c.buffer = 0;
        bss_0049d93c.count = 0;
        bss_0049d93c.capacity = 0;
    }
}
